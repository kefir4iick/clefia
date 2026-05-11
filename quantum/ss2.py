from qiskit import QuantumCircuit, QuantumRegister, ClassicalRegister
from qiskit_aer import AerSimulator
import matplotlib.pyplot as plt

qr_in = QuantumRegister(4, 'x')
qr_out = QuantumRegister(4, 'y')
anc = QuantumRegister(1, 'anc')
qc = QuantumCircuit(qr_in, qr_out, anc)

def compute_ss2_y3(qc, x, y, anc):
    qc.x(y[3])
    qc.cx(x[1], y[3])
    qc.ccx(x[0], x[1], y[3])
    qc.ccx(x[0], x[2], y[3])
    qc.ccx(x[0], x[3], y[3])
    qc.ccx(x[2], x[3], y[3])
    
    qc.ccx(x[0], x[1], anc[0])
    qc.ccx(anc[0], x[2], y[3])
    qc.ccx(x[0], x[1], anc[0])

def compute_ss2_y2(qc, x, y, anc):
    qc.cx(x[1], y[2])
    qc.cx(x[3], y[2])
    qc.ccx(x[0], x[2], y[2])
    qc.ccx(x[2], x[3], y[2])
    
    qc.ccx(x[0], x[1], anc[0])
    qc.ccx(anc[0], x[2], y[2])
    qc.ccx(x[0], x[1], anc[0])
    
    qc.ccx(x[0], x[2], anc[0])
    qc.ccx(anc[0], x[3], y[2])
    qc.ccx(x[0], x[2], anc[0])

def compute_ss2_y1(qc, x, y, anc):
    qc.x(y[1])
    qc.cx(x[0], y[1])
    qc.cx(x[1], y[1])
    qc.ccx(x[0], x[2], y[1])
    qc.ccx(x[0], x[3], y[1])
    qc.ccx(x[1], x[3], y[1])
    qc.ccx(x[2], x[3], y[1])
    
    qc.ccx(x[0], x[2], anc[0])
    qc.ccx(anc[0], x[3], y[1])
    qc.ccx(x[0], x[2], anc[0])

def compute_ss2_y0(qc, x, y, anc):
    qc.x(y[0])
    qc.cx(x[0], y[0])
    qc.cx(x[2], y[0])
    qc.ccx(x[0], x[2], y[0])
    qc.ccx(x[0], x[3], y[0])
    qc.ccx(x[1], x[3], y[0])
    qc.ccx(x[2], x[3], y[0])
    
    qc.ccx(x[0], x[1], anc[0])
    qc.ccx(anc[0], x[3], y[0])
    qc.ccx(x[0], x[1], anc[0])
    
    qc.ccx(x[1], x[2], anc[0])
    qc.ccx(anc[0], x[3], y[0])
    qc.ccx(x[1], x[2], anc[0])

compute_ss2_y3(qc, qr_in, qr_out, anc)
qc.barrier()
compute_ss2_y2(qc, qr_in, qr_out, anc)
qc.barrier()
compute_ss2_y1(qc, qr_in, qr_out, anc)
qc.barrier()
compute_ss2_y0(qc, qr_in, qr_out, anc)

qc.draw(output='mpl', filename='ss2.png', fold=30)

print("resource stats:")
ops = qc.count_ops()
for name, count in ops.items():
    print(f"gate {name}: {count}")
print(f"scheme depth: {qc.depth()}")


def test(original_qc, table):
    sim = AerSimulator()
    print("\ntest results: ")
    print("input (x3-x0) |  expected  |   get   |  status")
    print("-" * 45)
    
    for i in range(16):
        cr = ClassicalRegister(4, 'res')
        test_qc = QuantumCircuit(qr_in, qr_out, anc, cr)
        
        for bit in range(4):
            if (i >> bit) & 1:
                test_qc.x(qr_in[bit])
        
        test_qc.compose(original_qc, inplace=True)
        
        test_qc.measure(qr_out, cr)
        
        job = sim.run(test_qc, shots=1)
        result = job.result().get_counts()
        
        output_bin = list(result.keys())[0]
        output_val = int(output_bin, 2)
        
        status = "ok" if output_val == table[i] else "fail"
        in_bin = format(i, '04b')[::-1] 
        print(f"    {format(i, '04b')}      |     {format(table[i], '04b')}   |   {output_bin}   |   {status}")

tt = [0xb, 0x8, 0x5, 0xe, 0xa, 0x6, 0x4, 0xc, 0xf, 0x7, 0x2, 0x3, 0x1, 0x0, 0xd, 0x9]

test(qc, tt)
