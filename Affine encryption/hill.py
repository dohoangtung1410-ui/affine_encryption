import random
import math

# ---------------- Bảng ký tự 231 ----------------
char_list = (
    "abcdefghijklmnopqrstuvwxyz"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    " ~!@#$%^&*()-_+=|\\\"\n\t{[}]:;'<,>.?/"
    "0123456789"
    "áàảãạăắằẳẵặâấầẩẫậđ"
    "éèẻẽẹêếềểễệ"
    "íìỉĩị"
    "óòỏõọôốồổỗộơớờởỡợ"
    "úùủũụưứừửữự"
    "ýỳỷỹỵ"
    "ÁÀẢÃẠĂẮẰẲẴẶÂẤẦẨẪẬĐ"
    "ÉÈẺẼẸÊẾỀỂỄỆ"
    "ÍÌỈĨỊ"
    "ÓÒỎÕỌÔỐỒỔỖỘƠỚỜỞỠỢ"
    "ÚÙỦŨỤƯỨỪỬỮỰ"
    "ÝỲỶỸỴ"
)
CHAR_MOD = len(char_list)
char_to_index = {c: i for i, c in enumerate(char_list)}
index_to_char = {i: c for i, c in enumerate(char_list)}

# ---------------- Nghịch đảo modulo ----------------
def mod_inv(a, m):
    a = (a % m + m) % m
    if a == 0:
        raise ValueError("Không tìm được nghịch đảo modulo")
    t, new_t = 0, 1
    r, new_r = m, a
    while new_r != 0:
        q = r // new_r
        t, new_t = new_t, t - q * new_t
        r, new_r = new_r, r - q * new_r
    if r > 1:
        raise ValueError("Không tìm được nghịch đảo modulo")
    if t < 0:
        t += m
    return t

# ---------------- Định thức ----------------
def det_matrix(mat):
    n = len(mat)
    if n == 2:
        return mat[0][0]*mat[1][1] - mat[0][1]*mat[1][0]
    elif n == 3:
        return (mat[0][0]*(mat[1][1]*mat[2][2]-mat[1][2]*mat[2][1])
               - mat[0][1]*(mat[1][0]*mat[2][2]-mat[1][2]*mat[2][0])
               + mat[0][2]*(mat[1][0]*mat[2][1]-mat[1][1]*mat[2][0]))
    else:
        raise ValueError("Chỉ hỗ trợ ma trận 2x2 hoặc 3x3")

def gcd(a,b):
    while b:
        a,b = b, a%b
    return a

def kiem_tra_key(key):
    d = det_matrix(key)
    return gcd(d % CHAR_MOD, CHAR_MOD) == 1

# ---------------- Sinh key ----------------
def generate_key(n):
    while True:
        key = [[random.randint(-1000,1000) for _ in range(n)] for _ in range(n)]
        if kiem_tra_key(key):
            return key

# ---------------- Chuyển ký tự ↔ chỉ số ----------------
def text_to_indices(text):
    return [char_to_index.get(c, char_to_index['a']) for c in text]

def indices_to_text(indices):
    return ''.join(index_to_char[i] for i in indices)

# ---------------- Hill Cipher ----------------
def hill_ma_hoa(text, key):
    n = len(key)
    block = text_to_indices(text)
    original_len = len(block)
    while len(block) % n != 0:
        block.append(char_to_index['a'])
    cipher_block = []
    explanation = []
    for i in range(0, len(block), n):
        sub = block[i:i+n]
        temp = []
        for r in range(n):
            s = sum(key[r][c] * sub[c] for c in range(n))
            val = ((s % CHAR_MOD) + CHAR_MOD) % CHAR_MOD
            temp.append(val)
            explanation.append(f"Ký tự '{index_to_char[sub[r]]}' ở vị trí {i+r+1} "
                               f"được thay bằng '{index_to_char[val]}'")
        cipher_block.extend(temp)
    return indices_to_text(cipher_block), explanation, original_len

# ---------------- Nghịch đảo ma trận ----------------
def inv_matrix(key):
    n = len(key)
    det = det_matrix(key)
    det_inv = mod_inv(det, CHAR_MOD)
    inv_key = [[0]*n for _ in range(n)]
    if n == 2:
        inv_key[0][0] = ( key[1][1]*det_inv) % CHAR_MOD
        inv_key[0][1] = (-key[0][1]*det_inv) % CHAR_MOD
        inv_key[1][0] = (-key[1][0]*det_inv) % CHAR_MOD
        inv_key[1][1] = ( key[0][0]*det_inv) % CHAR_MOD
    elif n == 3:
        m = key
        inv_key[0][0] = ( m[1][1]*m[2][2]-m[1][2]*m[2][1])*det_inv % CHAR_MOD
        inv_key[0][1] = (-1*( m[0][1]*m[2][2]-m[0][2]*m[2][1])*det_inv) % CHAR_MOD
        inv_key[0][2] = ( m[0][1]*m[1][2]-m[0][2]*m[1][1])*det_inv % CHAR_MOD
        inv_key[1][0] = (-1*( m[1][0]*m[2][2]-m[1][2]*m[2][0])*det_inv) % CHAR_MOD
        inv_key[1][1] = ( m[0][0]*m[2][2]-m[0][2]*m[2][0])*det_inv % CHAR_MOD
        inv_key[1][2] = (-1*( m[0][0]*m[1][2]-m[0][2]*m[1][0])*det_inv) % CHAR_MOD
        inv_key[2][0] = ( m[1][0]*m[2][1]-m[1][1]*m[2][0])*det_inv % CHAR_MOD
        inv_key[2][1] = (-1*( m[0][0]*m[2][1]-m[0][1]*m[2][0])*det_inv) % CHAR_MOD
        inv_key[2][2] = ( m[0][0]*m[1][1]-m[0][1]*m[1][0])*det_inv % CHAR_MOD
        inv_key = [[(x+CHAR_MOD)%CHAR_MOD for x in row] for row in inv_key]
    return inv_key

# ---------------- Giải mã ----------------
def hill_giai_ma(text, key, original_len):
    n = len(key)
    inv_key = inv_matrix(key)
    block = text_to_indices(text)
    while len(block) % n != 0:
        block.append(char_to_index['a'])
    plain_block = []
    explanation = []
    for i in range(0, len(block), n):
        sub = block[i:i+n]
        temp = []
        for r in range(n):
            s = sum(inv_key[r][c] * sub[c] for c in range(n))
            val = ((s % CHAR_MOD) + CHAR_MOD) % CHAR_MOD
            temp.append(val)
            explanation.append(f"Ký tự '{index_to_char[sub[r]]}' ở vị trí {i+r+1} "
                               f"được giải mã thành '{index_to_char[val]}'")
        plain_block.extend(temp)
    return indices_to_text(plain_block)[:original_len], explanation

# ---------------- Menu chính ----------------
def print_key(key):
    for row in key:
        print(' '.join(map(str,row)))

def main():
    random.seed()
    key = []
    while True:
        print("\n===== HILL CIPHER MENU =====")
        print("1. Nhập key thủ công")
        print("2. Sinh key ngẫu nhiên")
        print("3. Mã hoá văn bản")
        print("4. Giải mã văn bản")
        print("5. Thoát")
        choice = input("Chọn (1-5): ").strip()
        if choice not in ['1','2','3','4','5']:
            print("Lựa chọn không hợp lệ.")
            continue
        if choice == '1':
            while True:
                n = int(input("Nhập kích thước key (2 hoặc 3): "))
                if n in [2,3]: break
            nums = list(map(int, input(f"Nhập {n*n} số nguyên cách nhau space: ").split()))
            key = [nums[i*n:(i+1)*n] for i in range(n)]
            if not kiem_tra_key(key):
                print("Key không hợp lệ (gcd(det,231)!=1)")
            else:
                print("Key đã lưu.")
        elif choice == '2':
            n = int(input("Nhập kích thước key (2 hoặc 3): "))
            key = generate_key(n)
            print("Key ngẫu nhiên hợp lệ:")
            print_key(key)
        elif choice == '3':
            if not key:
                print("Bạn cần nhập/sinh key trước!")
                continue
            text = input("Nhập văn bản cần mã hoá: ")
            cipher, explain, original_len = hill_ma_hoa(text, key)
            print("Bản mã:", cipher)
            print("Chi tiết từng ký tự:")
            for e in explain:
                print(e)
        elif choice == '4':
            if not key:
                print("Bạn cần nhập/sinh key trước!")
                continue
            text = input("Nhập văn bản cần giải mã: ")
            original_len = len(text)
            plain, explain = hill_giai_ma(text, key, original_len)
            print("Văn bản giải mã:", plain)
            print("Chi tiết từng ký tự:")
            for e in explain:
                print(e)
        else:
            print("Thoát chương trình.")
            break

if __name__ == "__main__":
    main()
