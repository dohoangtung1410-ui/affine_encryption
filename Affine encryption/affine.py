ALPHABET = [
    # Latin cơ bản
    'a','b','c','d','e','f','g','h','i','j','k','l','m','n',
    'o','p','q','r','s','t','u','v','w','x','y','z','A','B',
    'C','D','E','F','G','H','I','J','K','L','M','N','O','P',
    'Q','R','S','T','U','V','W','X','Y','Z',' ','~','`','!',
    '@','#','$','%','^','&','*','(',')','-','_','+','=','|',
    '\\','"','\n','\t','{','[',']','}',':',';','\'','<',',',
    '.','>','?','/','0','1','2','3','4','5','6','7','8','9',
    # Tiếng Việt - chữ thường
    'à','á','ả','ã','ạ','ă','ằ','ắ','ẳ','ẵ','ặ','â','ầ','ấ','ẩ','ẫ','ậ',
    'è','é','ẻ','ẽ','ẹ','ê','ề','ế','ể','ễ','ệ',
    'ì','í','ỉ','ĩ','ị',
    'ò','ó','ỏ','õ','ọ','ô','ồ','ố','ổ','ỗ','ộ','ơ','ờ','ớ','ở','ỡ','ợ',
    'ù','ú','ủ','ũ','ụ','ư','ừ','ứ','ử','ữ','ự',
    'ỳ','ý','ỷ','ỹ','ỵ',
    'đ',
    # Tiếng Việt - chữ hoa
    'À','Á','Ả','Ã','Ạ','Ă','Ằ','Ắ','Ẳ','Ẵ','Ặ','Â','Ầ','Ấ','Ẩ','Ẫ','Ậ',
    'È','É','Ẻ','Ẽ','Ẹ','Ê','Ề','Ế','Ể','Ễ','Ệ',
    'Ì','Í','Ỉ','Ĩ','Ị',
    'Ò','Ó','Ỏ','Õ','Ọ','Ô','Ồ','Ố','Ổ','Ỗ','Ộ','Ơ','Ờ','Ớ','Ở','Ỡ','Ợ',
    'Ù','Ú','Ủ','Ũ','Ụ','Ư','Ừ','Ứ','Ử','Ữ','Ự',
    'Ỳ','Ý','Ỷ','Ỹ','Ỵ',
    'Đ'
]


def build_maps(alphabet):
    index_of = {ch: idx for idx, ch in enumerate(alphabet)}
    char_of = {idx: ch for idx, ch in enumerate(alphabet)}
    return index_of, char_of


INDEX_OF, CHAR_OF = build_maps(ALPHABET)


class AffineCipher:
    
    def __init__(self, a, b):
        self.alphabet = ALPHABET
        self.index_of = INDEX_OF
        self.char_of = CHAR_OF
        self.modulus = len(self.alphabet)

        self.a_original = a
        self.b_original = b

        a = a % self.modulus
        b = b % self.modulus

        if not self._is_valid_key(a, b):
            raise ValueError(
                f"Khóa không hợp lệ! a phải nguyên tố cùng nhau với {self.modulus}"
            )
        
        self.a = a
        self.b = b
        self.a_inverse = self._mod_inverse(a, self.modulus)
    
    def _gcd(self, a, b):
        while b:
            a, b = b, a % b
        return a
    
    def _is_valid_key(self, a, b):
        return self._gcd(a, self.modulus) == 1
    
    def _mod_inverse(self, a, m):
        if self._gcd(a, m) != 1:
            return None
        
        def extended_gcd(a, b):
            if a == 0:
                return b, 0, 1
            gcd, x1, y1 = extended_gcd(b % a, a)
            x = y1 - (b // a) * x1
            y = x1
            return gcd, x, y
        
        gcd, x, y = extended_gcd(a, m)
        if gcd != 1:
            return None
        
        return (x % m + m) % m
    
    def encrypt(self, plaintext):
        ciphertext = ""
        for i, char in enumerate(plaintext):
            if char in self.index_of:
                x = self.index_of[char]
                y = (self.a * x + self.b) % self.modulus
                new_char = self.char_of[y]
                print(f"[{i}] '{char}' -> chỉ số {x} => y=(a*x+b) mod {self.modulus} = ({self.a}*{x}+{self.b}) mod {self.modulus} = {y} -> '{new_char}'")
                ciphertext += new_char
            else:
                print(f"[{i}] Ký tự không hỗ trợ: '{char}' (giữ nguyên)")
                ciphertext += char
        return ciphertext
    
    def decrypt(self, ciphertext):
        plaintext = ""
        for i, char in enumerate(ciphertext):
            if char in self.index_of:
                y = self.index_of[char]
                x = (self.a_inverse * (y - self.b)) % self.modulus
                new_char = self.char_of[x]
                print(f"[{i}] '{char}' -> chỉ số {y} => x=a^(-1)*(y-b) mod {self.modulus} = ({self.a_inverse}*({y}-{self.b})) mod {self.modulus} = {x} -> '{new_char}'")
                plaintext += new_char
            else:
                print(f"[{i}] Ký tự không hỗ trợ: '{char}' (giữ nguyên)")
                plaintext += char
        return plaintext

def display_menu():
    print("+" + "=" * 50 + "+")
    print("|" + " " * 15 + "MÃ HÓA AFFINE" + " " * 15 + "|")
    print("+" + "=" * 50 + "+")
    print("| 1. Nhập khóa (a,b) thủ công              |")
    print("| 2. Sinh khóa (a,b) tự động               |")
    print("| 3. Mã hóa                                |")
    print("| 4. Giải mã                               |")
    print("| 5. Thoát                                 |")
    print("+" + "=" * 50 + "+")

def get_valid_choice():
    while True:
        try:
            choice = input("Nhập lựa chọn (1-5): ")
            choice = int(choice)
            if 1 <= choice <= 5:
                return choice
            else:
                print("Lựa chọn phải từ 1 đến 5, vui lòng nhập lại!")
        except ValueError:
            print("Lựa chọn không hợp lệ, vui lòng nhập lại!")

def manual_key_input():
    while True:
        try:
            print("\n--- NHẬP KHÓA THỦ CÔNG ---")
            print("(Nhập 'q' hoặc 'quit' bất kỳ lúc nào để quay lại menu)")
            modulus = len(ALPHABET)
            print(f"Độ dài bảng ký tự (modulo) = {modulus}")
            valid_a_samples = [a for a in range(1, modulus) if AffineCipher._gcd(AffineCipher, a, modulus) == 1]
            preview_count = 20 if len(valid_a_samples) > 20 else len(valid_a_samples)
            preview = ", ".join(str(x) for x in valid_a_samples[:preview_count])
            more_note = " ..." if len(valid_a_samples) > preview_count else ""
            print("- a phải NGUYÊN TỐ CÙNG NHAU với modulo (gcd(a, modulo) = 1)")
            print(f"  Ví dụ a hợp lệ: {preview}{more_note}")
            print(f"- b có thể là BẤT KỲ số nguyên (kể cả số âm)")
            print(f"  Số âm sẽ được tự động chuẩn hóa về [0..{modulus-1}]")
            print(f"  Ví dụ: b=-10 sẽ thành b={(-10) % modulus}")
            
            # Nhập và kiểm tra a
            while True:
                try:
                    a_input = input(f"\nNhập giá trị a (có thể âm, nhưng sau chuẩn hóa không được chia hết cho 3,7,11): ")
                    if a_input.lower() in ['q', 'quit']:
                        print("Hủy nhập khóa, quay lại menu...")
                        return None
                    a = int(a_input)
                except ValueError:
                    print("Giá trị a phải là số nguyên. Vui lòng nhập lại!")
                    continue
                
                a_norm = a % modulus
                print(f"  → a sau chuẩn hóa: {a_norm}")
                
                if a_norm % 3 == 0 or a_norm % 7 == 0 or a_norm % 11 == 0:
                    print(f"  ✗ a={a_norm} chia hết cho 3, 7 hoặc 11. Vui lòng nhập lại!")
                    continue
                
                if AffineCipher._gcd(AffineCipher, a_norm, modulus) != 1:
                    print(f"  ✗ gcd({a_norm}, {modulus}) ≠ 1. Vui lòng nhập lại!")
                    continue
                
                print(f"  ✓ a={a_norm} hợp lệ!")
                break

            # Nhập b (chấp nhận mọi số nguyên)
            while True:
                try:
                    b_input = input(f"\nNhập giá trị b (bất kỳ số nguyên, kể cả số âm): ")
                    if b_input.lower() in ['q', 'quit']:
                        print("Hủy nhập khóa, quay lại menu...")
                        return None
                    b = int(b_input)
                    b_norm = b % modulus
                    print(f"  → b sau chuẩn hóa: {b_norm}")
                    print(f"  ✓ b={b_norm} hợp lệ!")
                    break
                except ValueError:
                    print("Giá trị b phải là số nguyên. Vui lòng nhập lại!")
                    continue
            
            cipher = AffineCipher(a, b)
            print(f"\n{'='*50}")
            print(f"Khóa đã nhập: a = {cipher.a_original}, b = {cipher.b_original}")
            print(f"Khóa sau chuẩn hóa: a = {cipher.a}, b = {cipher.b}")
            print(f"Nghịch đảo của {cipher.a} mod {cipher.modulus} = {cipher.a_inverse}")
            print(f"{'='*50}")
            return cipher
        except ValueError as e:
            print(f"Lỗi: {e}")
            print("Vui lòng nhập lại!")

def auto_key_generation():
    import random
    
    print("\n--- SINH KHÓA TỰ ĐỘNG ---")
    print("(Nhập 'q' hoặc 'quit' để quay lại menu)")
    confirm = input("Nhấn Enter để sinh khóa tự động, hoặc 'q' để hủy: ")
    
    if confirm.lower() in ['q', 'quit']:
        print("Hủy sinh khóa, quay lại menu...")
        return None
    
    modulus = len(ALPHABET)
    
    while True:
        # Random trong khoảng ±1 triệu
        a = random.randint(-10**6, 10**6)
        b = random.randint(-10**6, 10**6)

        # Chuẩn hóa về [0..modulus-1]
        a_norm = a % modulus
        b_norm = b % modulus

        # Kiểm tra tính hợp lệ: gcd(a, modulus) = 1
        if AffineCipher._gcd(AffineCipher, a_norm, modulus) == 1:
            try:
                cipher = AffineCipher(a, b)  # a, b gốc vẫn lưu lại
                print(f"\nKhóa được sinh (gốc): a = {a}, b = {b}")
                print(f"Khóa sau chuẩn hóa: a = {cipher.a}, b = {cipher.b}")
                print(f"Nghịch đảo của {cipher.a} mod {cipher.modulus} = {cipher.a_inverse}")
                return cipher
            except ValueError:
                continue  # Nếu lỡ fail thì thử lại

def encrypt_text(cipher):
    if cipher is None:
        print("Chưa có khóa! Vui lòng chọn chức năng 1 hoặc 2 trước.")
        return
    
    print("\n--- MÃ HÓA ---")
    print("(Nhập 'q' hoặc 'quit' để quay lại menu)")
    text = input("Nhập văn bản cần mã hóa: ")
    
    if text.lower() in ['q', 'quit']:
        print("Hủy mã hóa, quay lại menu...")
        return
    
    unsupported = [ch for ch in text if ch not in cipher.index_of]
    if unsupported:
        print("Cảnh báo: Có ký tự không hỗ trợ sẽ được giữ nguyên:")
        print(" ".join(repr(ch) for ch in unsupported))
    print("\nChi tiết từng ký tự:")
    encrypted = cipher.encrypt(text)
    print(f"\nKết quả mã hóa: {encrypted}")

def decrypt_text(cipher):
    if cipher is None:
        print("Chưa có khóa! Vui lòng chọn chức năng 1 hoặc 2 trước.")
        return
    
    print("\n--- GIẢI MÃ ---")
    print("(Nhập 'q' hoặc 'quit' để quay lại menu)")
    text = input("Nhập văn bản cần giải mã: ")
    
    if text.lower() in ['q', 'quit']:
        print("Hủy giải mã, quay lại menu...")
        return
    
    unsupported = [ch for ch in text if ch not in cipher.index_of]
    if unsupported:
        print("Cảnh báo: Có ký tự không hỗ trợ sẽ được giữ nguyên:")
        print(" ".join(repr(ch) for ch in unsupported))
    print("\nChi tiết từng ký tự:")
    decrypted = cipher.decrypt(text)
    print(f"\nKết quả giải mã: {decrypted}")

def main():
    cipher = None
    
    while True:
        display_menu()
        choice = get_valid_choice()
        
        if choice == 1:
            result = manual_key_input()
            if result is not None:
                cipher = result
        elif choice == 2:
            result = auto_key_generation()
            if result is not None:
                cipher = result
        elif choice == 3:
            encrypt_text(cipher)
        elif choice == 4:
            decrypt_text(cipher)
        elif choice == 5:
            print("\nCảm ơn bạn đã sử dụng chương trình!")
            break
        
        if choice != 5:
            input("\nNhấn Enter để tiếp tục...")
            print("\n" + "="*60 + "\n")

if __name__ == "__main__":
    main()