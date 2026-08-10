import sys
import re
import types
import ply.yacc as yacc

def generate_lalr1_tables(header_path: str, output_prefix: str = "parser_tables"):
    print(f"[*] Reading C header file: '{header_path}'...")
    try:
        with open(header_path, "r") as f:
            content = f.read()
    except FileNotFoundError:
        print(f"[!] Error: Could not find '{header_path}'. Check the path and try again.")
        sys.exit(1)

    # Extract all string literals matching "lhs -> rhs" pattern
    rule_strings = re.findall(r'"([^"\n]*?->[^"\n]*?)"', content)

    parsed_rules = []
    for rule_str in rule_strings:
        rule_str = rule_str.strip()
        if not rule_str or "->" not in rule_str:
            continue
            
        lhs, rhs = rule_str.split("->", 1)
        lhs = lhs.strip()
        rhs_tokens = tuple(rhs.strip().split()) if rhs.strip() else ()
        
        parsed_rules.append((lhs, rhs_tokens))

    if not parsed_rules:
        print("[!] Error: No grammar rules found in header file.")
        sys.exit(1)

    print(f"[+] Successfully extracted {len(parsed_rules)} grammar rules.")

    # Extract Terminals and Non-Terminals
    terminals = set()
    non_terminals = set()

    for lhs, rhs in parsed_rules:
        non_terminals.add(lhs)
        for sym in rhs:
            if sym.startswith("TOKEN_"):
                terminals.add(sym)
            else:
                non_terminals.add(sym)

    terminals.add("TOKEN_EOF")

    terminals_list = sorted(list(terminals))
    non_terminals_list = sorted(list(non_terminals))

    term_map = {t: i for i, t in enumerate(terminals_list)}
    nonterm_map = {nt: i for i, nt in enumerate(non_terminals_list)}

    start_symbol = parsed_rules[0][0]

    # Precedence specification to resolve operator ambiguity
    precedence = (
      
        ('right', 'TOKEN_ASSIGN', 'TOKEN_PLUS_ASSIGN', 'TOKEN_MINUS_ASSIGN', 'TOKEN_STAR_ASSIGN', 'TOKEN_SLASH_ASSIGN'),
        ('left', 'TOKEN_LOGICAL_OR', 'TOKEN_OR'),
        ('left', 'TOKEN_LOGICAL_AND', 'TOKEN_AND'),
        ('left', 'TOKEN_BIT_OR', 'TOKEN_PIPE'),
        ('left', 'TOKEN_BIT_XOR', 'TOKEN_CARET'),
        ('left', 'TOKEN_BIT_AND', 'TOKEN_AMPERSAND'),
        ('left', 'TOKEN_EQUAL', 'TOKEN_NOT_EQUAL'),
        ('left', 'TOKEN_LESS', 'TOKEN_LESS_EQUAL', 'TOKEN_GREATER', 'TOKEN_GREATER_EQUAL'),
        ('left', 'TOKEN_SHL', 'TOKEN_SHR'),
        ('left', 'TOKEN_PLUS', 'TOKEN_MINUS'),
        ('left', 'TOKEN_STAR', 'TOKEN_SLASH', 'TOKEN_PERCENT'),
        ('right', 'UNARY_OP'),
        ('left', 'TOKEN_DOT', 'TOKEN_COLON', 'TOKEN_SCOPE_RES'),
    )

    valid_prec = []
    for assoc, *toks in precedence:
        existing_toks = [t for t in toks if t in terminals]
        if existing_toks:
            valid_prec.append((assoc, *existing_toks))

    # Construct synthetic module for PLY inspection
    grammar_mod = types.ModuleType("grammar_mod")
    grammar_mod.__file__ = __file__
    sys.modules["grammar_mod"] = grammar_mod

    mod_dict = {
        'tokens': tuple(terminals_list),
        'start': start_symbol,
        'p_error': lambda p: None,
        '__file__': __file__,
        '__module__': "grammar_mod",
    }

    if valid_prec:
        mod_dict['precedence'] = tuple(valid_prec)

    for idx, (lhs, rhs) in enumerate(parsed_rules):
        rhs_str = " ".join(rhs) if rhs else ""
        doc_string = f"{lhs} : {rhs_str}"

        def make_handler(doc):
            def p_rule(p): pass
            p_rule.__doc__ = doc
            p_rule.__module__ = "grammar_mod"
            return p_rule

        mod_dict[f"p_rule_{idx}"] = make_handler(doc_string)

    grammar_mod.__dict__.update(mod_dict)

    print("[*] Generating LALR(1) state machine via PLY...")
    parser = yacc.yacc(module=grammar_mod, debug=True, write_tables=False)

    num_states = len(parser.action)
    num_terms = len(terminals_list)
    num_nonterms = len(non_terminals_list)

    print(f"[+] LALR(1) state machine built successfully! ({num_states} states generated)")

    # Build ACTION and GOTO matrices
    action_table = [[0 for _ in range(num_terms)] for _ in range(num_states)]
    goto_table = [[0 for _ in range(num_nonterms)] for _ in range(num_states)]

    for state_idx, actions in parser.action.items():
        for token, act_val in actions.items():
            if token in term_map:
                t_idx = term_map[token]
                if act_val > 0:
                    action_table[state_idx][t_idx] = act_val + 1  # Shift = state + 1
                elif act_val < 0:
                    action_table[state_idx][t_idx] = act_val      # Reduce = -(rule_id + 1)
                elif act_val == 0:
                    action_table[state_idx][t_idx] = 99999        # Accept

    for state_idx, gotos in parser.goto.items():
        for nonterm, target_state in gotos.items():
            if nonterm in nonterm_map:
                nt_idx = nonterm_map[nonterm]
                goto_table[state_idx][nt_idx] = target_state

    # Write C Header
    with open(f"{output_prefix}.h", "w") as f:
        f.write("#ifndef PARSER_TABLES_H\n")
        f.write("#define PARSER_TABLES_H\n\n")
        f.write("#include <stdint.h>\n\n")
        f.write(f"#define NUM_STATES {num_states}\n")
        f.write(f"#define NUM_TERMINALS {num_terms}\n")
        f.write(f"#define NUM_NON_TERMINALS {num_nonterms}\n\n")
        f.write("#define ACTION_ERROR 0\n")
        f.write("#define ACTION_ACCEPT 99999\n\n")
        f.write("#define IS_SHIFT(act) ((act) > 0 && (act) != ACTION_ACCEPT)\n")
        f.write("#define IS_REDUCE(act) ((act) < 0)\n")
        f.write("#define GET_SHIFT_STATE(act) ((act) - 1)\n")
        f.write("#define GET_REDUCE_RULE(act) (-(act) - 1)\n\n")

        f.write("/* Terminal Token Index Mapping:\n")
        for i, t in enumerate(terminals_list):
            f.write(f" * {i:3d}: {t}\n")
        f.write(" */\n\n")

        f.write("/* Non-Terminal Index Mapping:\n")
        for i, nt in enumerate(non_terminals_list):
            f.write(f" * {i:3d}: {nt}\n")
        f.write(" */\n\n")

        f.write("static const int32_t ACTION_TABLE[NUM_STATES][NUM_TERMINALS] = {\n")
        for row in action_table:
            f.write("    { " + ", ".join(f"{val:6d}" for val in row) + " },\n")
        f.write("};\n\n")

        f.write("static const int32_t GOTO_TABLE[NUM_STATES][NUM_NON_TERMINALS] = {\n")
        for row in goto_table:
            f.write("    { " + ", ".join(f"{val:6d}" for val in row) + " },\n")
        f.write("};\n\n")

        f.write("#endif // PARSER_TABLES_H\n")

    print(f"[+] Successfully exported C header to '{output_prefix}.h'")

    # Save detailed state output log
    import os
    if os.path.exists("parser.out"):
        os.rename("parser.out", f"{output_prefix}.log")
        print(f"[+] Exported detailed automaton log to '{output_prefix}.log'")

if __name__ == "__main__":
    header_file = sys.argv[1] if len(sys.argv) > 1 else "Parser/parse_structs.h"
    generate_lalr1_tables(header_file)