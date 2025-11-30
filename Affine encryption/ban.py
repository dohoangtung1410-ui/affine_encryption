from typing import Tuple
import math

# ===== BẢNG KÝ TỰ MỞ RỘNG =====
CHARSET = [
    'a','b','c','d','e','f','g','h','i','j','k','l','m','n',
    'o','p','q','r','s','t','u','v','w','x','y','z',
    'A','B','C','D','E','F','G','H','I','J','K','L','M','N',
    'O','P','Q','R','S','T','U','V','W','X','Y','Z',
    ' ','~','`','!','@','#','$','%','^','&','*','(',')','-','_','+','=','|',
    '\\','"','\n','\t','{','[',']','}',':',';','\'','<',',','.','>','?','/',
    '0','1','2','3','4','5','6','7','8','9',
    'Ă','Â','Ê','Ô','Ơ','Ư','Đ','ă','â','ê','ô','ơ','ư','đ',
    'À','Á','Ả','Ã','Ạ','à','á','ả','ã','ạ',
    'È','É','Ẻ','Ẽ','Ẹ','è','é','ẻ','ẽ','ẹ',
    'Ì','Í','Ỉ','Ĩ','Ị','ì','í','ỉ','ĩ','ị',
    'Ò','Ó','Ỏ','Õ','Ọ','ò','ó','ỏ','õ','ọ',
    'Ù','Ú','Ủ','Ũ','Ụ','ù','ú','ủ','ũ','ụ',
    'Ỳ','Ý','Ỷ','Ỹ','Ỵ','ỳ','ý','ỷ','ỹ','ỵ'
]
N = len(CHARSET)

# ===== NGHỊCH ĐẢO MOD =====
def modinv(a: int, m: int) -> int:
    a = a % m
    def egcd(x, y):
        if y == 0:
            return (x, 1, 0)
        g, s, t = egcd(y, x % y)
        return (g, t, s - (x // y) * t)
    g, s, _ = egcd(a, m)
    if g != 1:
        raise ValueError(f"Không tồn tại nghịch đảo modulo cho {a} mod {m}")
    return s % m

# ===== MÃ HOÁ =====
def encrypt_affine(plaintext: str, a: int, b: int) -> str:
    if math.gcd(a, N) != 1:
        raise ValueError("a không hợp lệ (phải nguyên tố cùng N).")
    ciphertext = []
    print("\n=== QUÁ TRÌNH MÃ HÓA ===")
    for ch in plaintext:
        if ch in CHARSET:
            x = CHARSET.index(ch)
            y = (a * x + b) % N
            new_ch = CHARSET[y]
            print(f"'{ch}' → '{new_ch}' (x={x}, y=({a}*{x}+{b}) % {N} = {y})")
            ciphertext.append(new_ch)
        else:
            print(f"'{ch}' không có trong bảng, giữ nguyên.")
            ciphertext.append(ch)
    return ''.join(ciphertext)

# ===== GIẢI MÃ =====
def decrypt_affine(ciphertext: str, a: int, b: int) -> str:
    if math.gcd(a, N) != 1:
        raise ValueError("a không hợp lệ (phải nguyên tố cùng N).")
    a_inv = modinv(a, N)
    plaintext = []
    print("\n=== QUÁ TRÌNH GIẢI MÃ ===")
    for ch in ciphertext:
        if ch in CHARSET:
            y = CHARSET.index(ch)
            x = (a_inv * (y - b)) % N
            new_ch = CHARSET[x]
            print(f"'{ch}' → '{new_ch}' (y={y}, x=({a_inv}*({y}-{b})) % {N} = {x})")
            plaintext.append(new_ch)
        else:
            print(f"'{ch}' không có trong bảng, giữ nguyên.")
            plaintext.append(ch)
    return ''.join(plaintext)

# ===== NHẬP KHÓA AN TOÀN =====
def safe_input_int(prompt: str) -> int:
    """Chỉ cho phép nhập số nguyên, không ký tự khác."""
    while True:
        value = input(prompt).strip()
        if not value.isdigit():
            print("❌ Lỗi: chỉ được nhập ký tự số!")
            continue
        return int(value)

# ===== CHẠY THỬ =====
if __name__ == "__main__":
    text = input("Nhập chuỗi (plaintext hoặc ciphertext): ").strip()

    # Nhập khóa (a, b) an toàn
    while True:
        a = safe_input_int(f"Nhập giá trị a (phải thỏa gcd(a, {N})=1): ")
        b = safe_input_int("Nhập giá trị b: ")
        if math.gcd(a, N) != 1:
            print(f"❌ a={a} không hợp lệ! gcd({a}, {N}) ≠ 1. Vui lòng nhập lại.\n")
        else:
            print("✅ Nhập khóa (a, b) hợp lệ! Khóa đã được chấp nhận.\n")
            break

    print("Chọn thao tác:\n1. Mã hóa\n2. Giải mã")
    choice = input("Nhập lựa chọn (1 hoặc 2): ").strip()

    try:
        if choice == "1":
            result = encrypt_affine(text, a, b)
            print("\nKết quả mã hóa:", result)
        elif choice == "2":
            result = decrypt_affine(text, a, b)
            print("\nKết quả giải mã:", result)
        else:
            print("Lựa chọn không hợp lệ!")
    except Exception as e:
        print(f"Lỗi: {e}")
