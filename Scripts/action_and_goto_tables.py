from dataclasses import dataclass
from typing import List, FrozenSet, Set

@dataclass(frozen=True)
class Rule:
    lhs: str
    rhs: tuple[str, ...]
    rule_id: int

@dataclass(frozen=True)
class LR1Item:
    rule: Rule
    dot: int
    lookahead: str

def compute_first_sets():
    return

def closure(items: Set[LR1Item]) -> Set[LR1Item]:
    return LR1Item

def goto(items: Set[LR1Item], symbol: str) -> Set[LR1Item]:
    return LR1Item

def build_automation(start_rule: Rule):
    # State 0 starts with the program entry point and EOF lookahead
    start_item = LR1Item(start_rule, dot=0, lookahead="EOF")
    state_0 = closure({start_item})
    
    states = [state_0]
    transitions = {} # Maps (state_index, symbol) -> next_state_index
    
    unprocessed = [0]
    
    while unprocessed:
        current_idx = unprocessed.pop(0)
        current_state = states[current_idx]
        
        # Find every unique symbol (terminal or non-terminal) immediately after a dot
        symbols_after_dot = {item.rule.rhs[item.dot] for item in current_state if item.dot < len(item.rule.rhs)}
        
        for symbol in symbols_after_dot:
            next_state = goto(current_state, symbol)
            
            if next_state not in states:
                states.append(next_state)
                unprocessed.append(len(states) - 1)
                
            transitions[(current_idx, symbol)] = states.index(next_state)
            
    return states, transitions