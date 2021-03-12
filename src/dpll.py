
def negate_literal(L):
    return L[1:] if L[0] == '!' else '!' + L

def unit_subsumption(P, u):
    return [ C for C in P if u not in C ]

def unit_resolution(P, u):
    n = negate_literal(u)
    return [ ([l for l in C if l != n]) for C in P ]

def simplify(F, L):
    return unit_resolution(unit_subsumption(F, L), L)

def get_unit(F):
    U = next((C for C in F if len(C) == 1), None)
    return U[0] if U else None

def get_literal(F):
    return F[0][0]

def contains_empty(F):
    return len([C for C in F if len(C) == 0]) > 0

def dpll(F):
    while True:
        L = get_unit(F)
        if not L:
            break
        F = simplify(F, L)

    if not F:
        return True
    if contains_empty(F):
        return False

    L = get_literal(F)
    return dpll(simplify(F, L)) or dpll(simplify(F, negate_literal(L)))


def unit_propagation(P, v):
    while True:
        u = get_unit(P)
        if not u:
            break
        v[u] = 1
        P = unit_subsumption(P, u)
        P = unit_resolution(P, u)

    return P, v

# F = [ ('A', 'B'), ('!A', 'C'), ('B', '!C'), ('!A', '!B', '!C') ]
# F = [ ('!A', 'B'), ('!B', 'C'), ('A', 'B', 'C'), ('A', '!C'), ('!A', '!B', '!C') ]

# F = [ ('!x', 'y', '!z'), ('!x', 'z'), ('!y', 'x'), ('y') ]
F = [ ('u', '!w'), ('u', '!z'), ('w', '!y'), ('w', '!x'), ('x', 'y', 'z') ]

print(F)
if dpll(F):
    print("F ist erfüllbar.")
else:
    print("F ist unerfüllbar.")

P = [ ('!x', 'y', '!z'), ('!x', 'z'), ('!y', 'x'), ('y') ]
v = dict()

print(P)

P, v = unit_propagation(P, v)

print(v)