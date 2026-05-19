from qiskit import QuantumCircuit, QuantumRegister, ClassicalRegister
from qiskit_aer import AerSimulator

qr_in  = QuantumRegister(8, 'x')   
qr_mid = QuantumRegister(8, 't')
qr_u   = QuantumRegister(8, 'u')
qr_out = QuantumRegister(8, 'y')
anc    = QuantumRegister(1, 'anc')
cr     = ClassicalRegister(8, 'res')

qc = QuantumCircuit(qr_in, qr_mid, qr_u, qr_out, anc, cr)

def compute_ss0(qc, x, y, anc):
    qc.x(y[3])
    qc.cx(x[0], y[3]); qc.ccx(x[0], x[1], y[3]); qc.ccx(x[1], x[2], y[3])
    qc.ccx(x[1], x[3], y[3]); qc.ccx(x[2], x[3], y[3])
    qc.ccx(x[0], x[1], anc[0]); qc.ccx(anc[0], x[2], y[3]); qc.ccx(x[0], x[1], anc[0])
    qc.ccx(x[1], x[2], anc[0]); qc.ccx(anc[0], x[3], y[3]); qc.ccx(x[1], x[2], anc[0])

    qc.x(y[2])
    qc.cx(x[2], y[2]); qc.cx(x[3], y[2]); qc.ccx(x[0], x[1], y[2])
    qc.ccx(x[0], x[2], y[2]); qc.ccx(x[1], x[3], y[2])
    qc.ccx(x[0], x[1], anc[0]); qc.ccx(anc[0], x[2], y[2]); qc.ccx(x[0], x[1], anc[0])
    qc.ccx(x[1], x[2], anc[0]); qc.ccx(anc[0], x[3], y[2]); qc.ccx(x[1], x[2], anc[0])

    qc.x(y[1])
    qc.cx(x[1], y[1]); qc.cx(x[2], y[1]); qc.ccx(x[0], x[1], y[1])
    qc.ccx(x[0], x[2], y[1]); qc.ccx(x[0], x[3], y[1])
    qc.ccx(x[1], x[2], anc[0]); qc.ccx(anc[0], x[3], y[1]); qc.ccx(x[1], x[2], anc[0])

    qc.cx(x[3], y[0]); qc.ccx(x[0], x[2], y[0]); qc.ccx(x[1], x[3], y[0])
    qc.ccx(x[0], x[2], anc[0]); qc.ccx(anc[0], x[3], y[0]); qc.ccx(x[0], x[2], anc[0])
    qc.ccx(x[1], x[2], anc[0]); qc.ccx(anc[0], x[3], y[0]); qc.ccx(x[1], x[2], anc[0])


def compute_ss1(qc, x, y, anc):
    qc.cx(x[3], y[3])
    qc.ccx(x[0], x[1], y[3]); qc.ccx(x[0], x[2], y[3]); qc.ccx(x[1], x[2], y[3])
    qc.ccx(x[0], x[1], anc[0]); qc.ccx(anc[0], x[2], y[3]); qc.ccx(x[0], x[1], anc[0])
    qc.ccx(x[0], x[1], anc[0]); qc.ccx(anc[0], x[3], y[3]); qc.ccx(x[0], x[1], anc[0])

    qc.x(y[2])
    qc.cx(x[1], y[2]); qc.cx(x[2], y[2]); qc.cx(x[3], y[2])
    qc.ccx(x[0], x[1], y[2]); qc.ccx(x[1], x[2], y[2]); qc.ccx(x[0], x[3], y[2]); qc.ccx(x[2], x[3], y[2])
    qc.ccx(x[0], x[1], anc[0]); qc.ccx(anc[0], x[2], y[2]); qc.ccx(x[0], x[1], anc[0])
    qc.ccx(x[1], x[2], anc[0]); qc.ccx(anc[0], x[3], y[2]); qc.ccx(x[1], x[2], anc[0])

    qc.x(y[1])
    qc.cx(x[0], y[1]); qc.cx(x[1], y[1]); qc.cx(x[3], y[1])
    qc.ccx(x[0], x[1], y[1]); qc.ccx(x[0], x[2], y[1]); qc.ccx(x[1], x[2], y[1]); qc.ccx(x[0], x[3], y[1])
    qc.ccx(x[0], x[1], anc[0]); qc.ccx(anc[0], x[2], y[1]); qc.ccx(x[0], x[1], anc[0])
    qc.ccx(x[0], x[1], anc[0]); qc.ccx(anc[0], x[3], y[1]); qc.ccx(x[0], x[1], anc[0])

    qc.cx(x[3], y[0])
    qc.ccx(x[0], x[1], y[0]); qc.ccx(x[0], x[2], y[0]); qc.ccx(x[0], x[3], y[0])
    qc.ccx(x[1], x[3], y[0]); qc.ccx(x[2], x[3], y[0])
    qc.ccx(x[0], x[1], anc[0]); qc.ccx(anc[0], x[2], y[0]); qc.ccx(x[0], x[1], anc[0])
    qc.ccx(x[0], x[1], anc[0]); qc.ccx(anc[0], x[3], y[0]); qc.ccx(x[0], x[1], anc[0])
    qc.ccx(x[0], x[2], anc[0]); qc.ccx(anc[0], x[3], y[0]); qc.ccx(x[0], x[2], anc[0])


def compute_ss2(qc, x, y, anc):
    qc.x(y[3])
    qc.cx(x[1], y[3]); qc.ccx(x[0], x[1], y[3]); qc.ccx(x[0], x[2], y[3])
    qc.ccx(x[0], x[3], y[3]); qc.ccx(x[2], x[3], y[3])
    qc.ccx(x[0], x[1], anc[0]); qc.ccx(anc[0], x[2], y[3]); qc.ccx(x[0], x[1], anc[0])

    qc.cx(x[1], y[2]); qc.cx(x[3], y[2]); qc.ccx(x[0], x[2], y[2]); qc.ccx(x[2], x[3], y[2])
    qc.ccx(x[0], x[1], anc[0]); qc.ccx(anc[0], x[2], y[2]); qc.ccx(x[0], x[1], anc[0])
    qc.ccx(x[0], x[2], anc[0]); qc.ccx(anc[0], x[3], y[2]); qc.ccx(x[0], x[2], anc[0])

    qc.x(y[1])
    qc.cx(x[0], y[1]); qc.cx(x[1], y[1]); qc.ccx(x[0], x[2], y[1]); qc.ccx(x[0], x[3], y[1])
    qc.ccx(x[1], x[3], y[1]); qc.ccx(x[2], x[3], y[1])
    qc.ccx(x[0], x[2], anc[0]); qc.ccx(anc[0], x[3], y[1]); qc.ccx(x[0], x[2], anc[0])

    qc.x(y[0])
    qc.cx(x[0], y[0]); qc.cx(x[2], y[0]); qc.ccx(x[0], x[2], y[0]); qc.ccx(x[0], x[3], y[0])
    qc.ccx(x[1], x[3], y[0]); qc.ccx(x[2], x[3], y[0])
    qc.ccx(x[0], x[1], anc[0]); qc.ccx(anc[0], x[3], y[0]); qc.ccx(x[0], x[1], anc[0])
    qc.ccx(x[1], x[2], anc[0]); qc.ccx(anc[0], x[3], y[0]); qc.ccx(x[1], x[2], anc[0])


def compute_ss3(qc, x, y, anc):
    qc.x(y[3])
    qc.cx(x[0], y[3]); qc.cx(x[1], y[3]); qc.cx(x[2], y[3]); qc.cx(x[3], y[3])
    qc.ccx(x[0], x[2], y[3]); qc.ccx(x[1], x[2], y[3]); qc.ccx(x[0], x[3], y[3])
    qc.ccx(x[0], x[1], anc[0]); qc.ccx(anc[0], x[2], y[3]); qc.ccx(x[0], x[1], anc[0])
    qc.ccx(x[0], x[2], anc[0]); qc.ccx(anc[0], x[3], y[3]); qc.ccx(x[0], x[2], anc[0])

    qc.cx(x[1], y[2]); qc.ccx(x[0], x[2], y[2]); qc.ccx(x[0], x[3], y[2]); qc.ccx(x[1], x[3], y[2])
    qc.ccx(x[0], x[1], anc[0]); qc.ccx(anc[0], x[2], y[2]); qc.ccx(x[0], x[1], anc[0])
    qc.ccx(x[0], x[1], anc[0]); qc.ccx(anc[0], x[3], y[2]); qc.ccx(x[0], x[1], anc[0])
    qc.ccx(x[0], x[2], anc[0]); qc.ccx(anc[0], x[3], y[2]); qc.ccx(x[0], x[2], anc[0])
    qc.ccx(x[1], x[2], anc[0]); qc.ccx(anc[0], x[3], y[2]); qc.ccx(x[1], x[2], anc[0])

    qc.x(y[1])
    qc.cx(x[3], y[1]); qc.ccx(x[0], x[1], y[1]); qc.ccx(x[0], x[2], y[1])
    qc.ccx(x[1], x[2], y[1]); qc.ccx(x[0], x[3], y[1]); qc.ccx(x[2], x[3], y[1])
    qc.ccx(x[0], x[1], anc[0]); qc.ccx(anc[0], x[2], y[1]); qc.ccx(x[0], x[1], anc[0])

    qc.cx(x[2], y[0]); qc.ccx(x[0], x[1], y[0]); qc.ccx(x[0], x[2], y[0]); qc.ccx(x[0], x[3], y[0])
    qc.ccx(x[0], x[1], anc[0]); qc.ccx(anc[0], x[2], y[0]); qc.ccx(x[0], x[1], anc[0])
    qc.ccx(x[0], x[1], anc[0]); qc.ccx(anc[0], x[3], y[0]); qc.ccx(x[0], x[1], anc[0])
    qc.ccx(x[1], x[2], anc[0]); qc.ccx(anc[0], x[3], y[0]); qc.ccx(x[1], x[2], anc[0])


def gf_mul2(qc, src, target):
    qc.cx(src[2], target[3])
    qc.cx(src[1], target[2])
    qc.cx(src[0], target[1])
    qc.cx(src[3], target[1]) 
    qc.cx(src[3], target[0])


def build_S0_circuit():
    compute_ss0(qc, qr_in[4:8], qr_mid[0:4], anc)   
    compute_ss1(qc, qr_in[0:4], qr_mid[4:8], anc)   

    qc.barrier()

    for i in range(4):
        qc.cx(qr_mid[i], qr_u[i])
    gf_mul2(qc, qr_mid[4:8], qr_u[0:4])

    for i in range(4):
        qc.cx(qr_mid[4+i], qr_u[4+i])
    gf_mul2(qc, qr_mid[0:4], qr_u[4:8])

    qc.barrier()

    compute_ss2(qc, qr_u[0:4], qr_out[4:8], anc)   
    compute_ss3(qc, qr_u[4:8], qr_out[0:4], anc)   

    qc.measure(qr_out, cr)


build_S0_circuit()

print("resource stats:")
ops = qc.count_ops()
for g, c in sorted(ops.items()):
    print(f"{g:8}: {c}")
print(f"scheme depth: {qc.depth()}")

s0_table = [0x57,0x49,0xd1,0xc6,0x2f,0x33,0x74,0xfb,0x95,0x6d,0x82,0xea,0x0e,0xb0,0xa8,0x1c,
0x28,0xd0,0x4b,0x92,0x5c,0xee,0x85,0xb1,0xc4,0x0a,0x76,0x3d,0x63,0xf9,0x17,0xaf,
0xbf,0xa1,0x19,0x65,0xf7,0x7a,0x32,0x20,0x06,0xce,0xe4,0x83,0x9d,0x5b,0x4c,0xd8,
0x42,0x5d,0x2e,0xe8,0xd4,0x9b,0x0f,0x13,0x3c,0x89,0x67,0xc0,0x71,0xaa,0xb6,0xf5,
0xa4,0xbe,0xfd,0x8c,0x12,0x00,0x97,0xda,0x78,0xe1,0xcf,0x6b,0x39,0x43,0x55,0x26,
0x30,0x98,0xcc,0xdd,0xeb,0x54,0xb3,0x8f,0x4e,0x16,0xfa,0x22,0xa5,0x77,0x09,0x61,
0xd6,0x2a,0x53,0x37,0x45,0xc1,0x6c,0xae,0xef,0x70,0x08,0x99,0x8b,0x1d,0xf2,0xb4,
0xe9,0xc7,0x9f,0x4a,0x31,0x25,0xfe,0x7c,0xd3,0xa2,0xbd,0x56,0x14,0x88,0x60,0x0b,
0xcd,0xe2,0x34,0x50,0x9e,0xdc,0x11,0x05,0x2b,0xb7,0xa9,0x48,0xff,0x66,0x8a,0x73,
0x03,0x75,0x86,0xf1,0x6a,0xa7,0x40,0xc2,0xb9,0x2c,0xdb,0x1f,0x58,0x94,0x3e,0xed,
0xfc,0x1b,0xa0,0x04,0xb8,0x8d,0xe6,0x59,0x62,0x93,0x35,0x7e,0xca,0x21,0xdf,0x47,
0x15,0xf3,0xba,0x7f,0xa6,0x69,0xc8,0x4d,0x87,0x3b,0x9c,0x01,0xe0,0xde,0x24,0x52,
0x7b,0x0c,0x68,0x1e,0x80,0xb2,0x5a,0xe7,0xad,0xd5,0x23,0xf4,0x46,0x3f,0x91,0xc9,
0x6e,0x84,0x72,0xbb,0x0d,0x18,0xd9,0x96,0xf0,0x5f,0x41,0xac,0x27,0xc5,0xe3,0x3a,
0x81,0x6f,0x07,0xa3,0x79,0xf6,0x2d,0x38,0x1a,0x44,0x5e,0xb5,0xd2,0xec,0xcb,0x90,
0x9a,0x36,0xe5,0x29,0xc3,0x4f,0xab,0x64,0x51,0xf8,0x10,0xd7,0xbc,0x02,0x7d,0x8e]

def validate_s0():
    sim = AerSimulator(method='matrix_product_state')
    print("\nvalidation s0: ")
    success = 0
    for i in range(256):
        test = QuantumCircuit(qr_in, qr_mid, qr_u, qr_out, anc, cr)

        for b in range(8):
            if (i >> b) & 1:
                test.x(qr_in[b])  

        test.compose(qc, inplace=True)
        result = sim.run(test, shots=1).result().get_counts()
        out_str = list(result.keys())[0]
        got = int(out_str, 2)

        expected = s0_table[i]
        if got == expected:
            success += 1
        elif i < 10:
            print(f"0x{i:02X}  Exp: 0x{expected:02X}  Got: 0x{got:02X}  FAIL")

    print(f"\nresult: {success}/256 success")


validate_s0()
