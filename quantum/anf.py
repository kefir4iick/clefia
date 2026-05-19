#truth_table = [0xe, 0x6, 0xc, 0xa, 0x8, 0x7, 0x2, 0xf, 0xb, 0x1, 0x4, 0x0, 0x5, 0x9, 0xd, 0x3]
#truth_table = [0x6, 0x4, 0x0, 0xd, 0x2, 0xb, 0xa, 0x3, 0x9, 0xc, 0xe, 0xf, 0x8, 0x7, 0x5, 0x1]
#truth_table = [0xb, 0x8, 0x5, 0xe, 0xa, 0x6, 0x4, 0xc, 0xf, 0x7, 0x2, 0x3, 0x1, 0x0, 0xd, 0x9]
truth_table = [0xa, 0x2, 0x6, 0xd, 0x3, 0x4, 0x5, 0xe, 0x0, 0x7, 0x8, 0x9, 0xb, 0xf, 0xc, 0x1] 

def get_anf_coeffs(table):
    anf = list(table)
    n = 4
    for i in range(n):
        for j in range(1 << n):
            if j & (1 << i):
                anf[j] ^= anf[j ^ (1 << i)]
    return anf

anf_coeffs = get_anf_coeffs(truth_table)

print("equations:")
for bit_idx in range(4): 
    terms = []
    for i in range(16):
        if (anf_coeffs[i] >> (3 - bit_idx)) & 1:
            vars = []
            for v in range(4):
                if (i >> v) & 1:
                    vars.append(f"x{v}")
            term = "*".join(vars) if vars else "1"
            terms.append(term)
    print(f"y{3-bit_idx} = " + " ⊕ ".join(terms))


def check(anf):
    print("\ntest: ")
    for x in range(16):
        calculated_y = 0
        for bit_pos in range(4): 
            bit_val = 0
            for i in range(16):
                if (anf[i] >> (3 - bit_pos)) & 1:
                    if (x & i) == i:
                        bit_val ^= 1
            calculated_y |= (bit_val << (3 - bit_pos))
        
        if calculated_y != truth_table[x]:
            print(f"error on input {x:04b}: expected {truth_table[x]:04b}, get {calculated_y:04b}")
    print("success")

check(anf_coeffs)
