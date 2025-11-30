#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <random>
#include <codecvt>
#include <locale>

using namespace std;

// Chuyển đổi UTF-8 string thành wstring
wstring toWString(const string& str) {
    wstring_convert<codecvt_utf8<wchar_t>> converter;
    return converter.from_bytes(str);
}

// Chuyển đổi wstring thành UTF-8 string
string toString(const wstring& wstr) {
    wstring_convert<codecvt_utf8<wchar_t>> converter;
    return converter.to_bytes(wstr);
}

class AffineCipher {
private:
    vector<wchar_t> alphabet;
    map<wchar_t, int> indexOf;
    map<int, wchar_t> charOf;
    int modulus;
    int a, b;
    int aOriginal, bOriginal;
    int aInverse;

    void buildMaps() {
        for (int i = 0; i < alphabet.size(); i++) {
            indexOf[alphabet[i]] = i;
            charOf[i] = alphabet[i];
        }
        modulus = alphabet.size();
    }

    int gcd(int a, int b) {
        while (b) {
            int temp = b;
            b = a % b;
            a = temp;
        }
        return a;
    }

    bool isValidKey(int a, int b) {
        return gcd(a, modulus) == 1;
    }

    int modInverse(int a, int m) {
        if (gcd(a, m) != 1) return -1;
        
        int m0 = m, x0 = 0, x1 = 1;
        while (a > 1) {
            int q = a / m;
            int t = m;
            m = a % m;
            a = t;
            t = x0;
            x0 = x1 - q * x0;
            x1 = t;
        }
        if (x1 < 0) x1 += m0;
        return x1;
    }

    void initAlphabet() {
        // Latin cơ bản
        string basic = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ ~`!@#$%^&*()-_+=|\\\"\\n\\t{[]}:;'<,.>?/0123456789";
        wstring wbasic = toWString(basic);
        for (wchar_t c : wbasic) {
            alphabet.push_back(c);
        }

        // Tiếng Việt - chữ thường
        wstring vnLower = L"àáảãạăằắẳẵặâầấẩẫậèéẻẽẹêềếểễệìíỉĩịòóỏõọôồốổỗộơờớởỡợùúủũụưừứửữựỳýỷỹỵđ";
        for (wchar_t c : vnLower) {
            alphabet.push_back(c);
        }

        // Tiếng Việt - chữ hoa
        wstring vnUpper = L"ÀÁẢÃẠĂẰẮẲẴẶÂẦẤẨẪẬÈÉẺẼẸÊỀẾỂỄỆÌÍỈĨỊÒÓỎÕỌÔỒỐỔỖỘƠỜỚỞỠỢÙÚỦŨỤƯỪỨỬỮỰỲÝỶỸỴĐ";
        for (wchar_t c : vnUpper) {
            alphabet.push_back(c);
        }
    }

public:
    AffineCipher(int a, int b) {
        initAlphabet();
        buildMaps();

        aOriginal = a;
        bOriginal = b;

        // Chuẩn hóa
        this->a = ((a % modulus) + modulus) % modulus;
        this->b = ((b % modulus) + modulus) % modulus;

        if (!isValidKey(this->a, this->b)) {
            throw invalid_argument("Khóa không hợp lệ! a phải nguyên tố cùng nhau với " + to_string(modulus));
        }

        aInverse = modInverse(this->a, modulus);
    }

    wstring encrypt(const wstring& plaintext) {
        wstring ciphertext = L"";
        for (int i = 0; i < plaintext.length(); i++) {
            wchar_t ch = plaintext[i];
            if (indexOf.count(ch)) {
                int x = indexOf[ch];
                int y = (a * x + b) % modulus;
                wchar_t newChar = charOf[y];
                wcout << L"[" << i << L"] '" << ch << L"' -> chỉ số " << x 
                      << L" => y=(a*x+b) mod " << modulus 
                      << L" = (" << a << L"*" << x << L"+" << b << L") mod " 
                      << modulus << L" = " << y << L" -> '" << newChar << L"'" << endl;
                ciphertext += newChar;
            } else {
                wcout << L"[" << i << L"] Ký tự không hỗ trợ: '" << ch << L"' (giữ nguyên)" << endl;
                ciphertext += ch;
            }
        }
        return ciphertext;
    }

    wstring decrypt(const wstring& ciphertext) {
        wstring plaintext = L"";
        for (int i = 0; i < ciphertext.length(); i++) {
            wchar_t ch = ciphertext[i];
            if (indexOf.count(ch)) {
                int y = indexOf[ch];
                int x = (aInverse * (y - b + modulus)) % modulus;
                wchar_t newChar = charOf[x];
                wcout << L"[" << i << L"] '" << ch << L"' -> chỉ số " << y 
                      << L" => x=a^(-1)*(y-b) mod " << modulus 
                      << L" = (" << aInverse << L"*(" << y << L"-" << b << L")) mod " 
                      << modulus << L" = " << x << L" -> '" << newChar << L"'" << endl;
                plaintext += newChar;
            } else {
                wcout << L"[" << i << L"] Ký tự không hỗ trợ: '" << ch << L"' (giữ nguyên)" << endl;
                plaintext += ch;
            }
        }
        return plaintext;
    }

    int getA() const { return a; }
    int getB() const { return b; }
    int getAOriginal() const { return aOriginal; }
    int getBOriginal() const { return bOriginal; }
    int getAInverse() const { return aInverse; }
    int getModulus() const { return modulus; }

    vector<int> getValidAValues() {
        vector<int> validA;
        for (int i = 1; i < modulus; i++) {
            if (gcd(i, modulus) == 1) {
                validA.push_back(i);
            }
        }
        return validA;
    }

    static int staticGcd(int a, int b) {
        while (b) {
            int temp = b;
            b = a % b;
            a = temp;
        }
        return a;
    }
};

void displayMenu() {
    wcout << L"+" << wstring(50, L'=') << L"+" << endl;
    wcout << L"|" << wstring(15, L' ') << L"MÃ HÓA AFFINE" << wstring(15, L' ') << L"|" << endl;
    wcout << L"+" << wstring(50, L'=') << L"+" << endl;
    wcout << L"| 1. Nhập khóa (a,b) thủ công              |" << endl;
    wcout << L"| 2. Sinh khóa (a,b) tự động               |" << endl;
    wcout << L"| 3. Mã hóa                                |" << endl;
    wcout << L"| 4. Giải mã                               |" << endl;
    wcout << L"| 5. Thoát                                 |" << endl;
    wcout << L"+" << wstring(50, L'=') << L"+" << endl;
}

int getValidChoice() {
    int choice;
    while (true) {
        wcout << L"Nhập lựa chọn (1-5): ";
        if (wcin >> choice && choice >= 1 && choice <= 5) {
            wcin.ignore();
            return choice;
        }
        wcout << L"Lựa chọn không hợp lệ, vui lòng nhập lại!" << endl;
        wcin.clear();
        wcin.ignore(10000, '\n');
    }
}

AffineCipher* manualKeyInput(int modulus) {
    wcout << L"\n--- NHẬP KHÓA THỦ CÔNG ---" << endl;
    wcout << L"Độ dài bảng ký tự (modulo) = " << modulus << endl;

    vector<int> validA;
    for (int i = 1; i < modulus; i++) {
        if (AffineCipher::staticGcd(i, modulus) == 1) {
            validA.push_back(i);
        }
    }

    wcout << L"- a phải NGUYÊN TỐ CÙNG NHAU với modulo (gcd(a, modulo) = 1)" << endl;
    wcout << L"  Ví dụ a hợp lệ: ";
    int previewCount = min(20, (int)validA.size());
    for (int i = 0; i < previewCount; i++) {
        wcout << validA[i];
        if (i < previewCount - 1) wcout << L", ";
    }
    if (validA.size() > 20) wcout << L" ...";
    wcout << endl;
    wcout << L"- b có thể là BẤT KỲ số nguyên (kể cả số âm)" << endl;

    int a, b;
    while (true) {
        wcout << L"\nNhập giá trị a: ";
        wcin >> a;
        int aNorm = ((a % modulus) + modulus) % modulus;
        wcout << L"  → a sau chuẩn hóa: " << aNorm << endl;

        if (aNorm % 3 == 0 || aNorm % 7 == 0 || aNorm % 11 == 0) {
            wcout << L"  ✗ a=" << aNorm << L" chia hết cho 3, 7 hoặc 11. Vui lòng nhập lại!" << endl;
            continue;
        }

        if (AffineCipher::staticGcd(aNorm, modulus) != 1) {
            wcout << L"  ✗ gcd(" << aNorm << L", " << modulus << L") ≠ 1. Vui lòng nhập lại!" << endl;
            continue;
        }

        wcout << L"  ✓ a=" << aNorm << L" hợp lệ!" << endl;
        break;
    }

    wcout << L"\nNhập giá trị b: ";
    wcin >> b;
    int bNorm = ((b % modulus) + modulus) % modulus;
    wcout << L"  → b sau chuẩn hóa: " << bNorm << endl;
    wcout << L"  ✓ b=" << bNorm << L" hợp lệ!" << endl;

    AffineCipher* cipher = new AffineCipher(a, b);
    wcout << L"\n" << wstring(50, L'=') << endl;
    wcout << L"Khóa đã nhập: a = " << cipher->getAOriginal() << L", b = " << cipher->getBOriginal() << endl;
    wcout << L"Khóa sau chuẩn hóa: a = " << cipher->getA() << L", b = " << cipher->getB() << endl;
    wcout << L"Nghịch đảo của " << cipher->getA() << L" mod " << cipher->getModulus() 
          << L" = " << cipher->getAInverse() << endl;
    wcout << wstring(50, L'=') << endl;

    return cipher;
}

AffineCipher* autoKeyGeneration(int modulus) {
    vector<int> validA;
    for (int i = 1; i < modulus; i++) {
        if (AffineCipher::staticGcd(i, modulus) == 1) {
            validA.push_back(i);
        }
    }

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> disA(0, validA.size() - 1);
    uniform_int_distribution<> disB(0, modulus - 1);

    int a = validA[disA(gen)];
    int b = disB(gen);

    AffineCipher* cipher = new AffineCipher(a, b);
    wcout << L"\n--- KHÓA TỰ ĐỘNG ---" << endl;
    wcout << L"Khóa được sinh: a = " << cipher->getA() << L", b = " << cipher->getB() << endl;
    wcout << L"Nghịch đảo của " << cipher->getA() << L" mod " << cipher->getModulus() 
          << L" = " << cipher->getAInverse() << endl;

    return cipher;
}

void encryptText(AffineCipher* cipher) {
    if (cipher == nullptr) {
        wcout << L"Chưa có khóa! Vui lòng chọn chức năng 1 hoặc 2 trước." << endl;
        return;
    }

    wcout << L"\n--- MÃ HÓA ---" << endl;
    wcout << L"Nhập văn bản cần mã hóa: ";
    wstring text;
    getline(wcin, text);

    wcout << L"\nChi tiết từng ký tự:" << endl;
    wstring encrypted = cipher->encrypt(text);
    wcout << L"\nKết quả mã hóa: " << encrypted << endl;
}

void decryptText(AffineCipher* cipher) {
    if (cipher == nullptr) {
        wcout << L"Chưa có khóa! Vui lòng chọn chức năng 1 hoặc 2 trước." << endl;
        return;
    }

    wcout << L"\n--- GIẢI MÃ ---" << endl;
    wcout << L"Nhập văn bản cần giải mã: ";
    wstring text;
    getline(wcin, text);

    wcout << L"\nChi tiết từng ký tự:" << endl;
    wstring decrypted = cipher->decrypt(text);
    wcout << L"\nKết quả giải mã: " << decrypted << endl;
}

int main() {
    // Thiết lập locale cho tiếng Việt
    locale::global(locale(""));
    wcout.imbue(locale());
    wcin.imbue(locale());

    AffineCipher* cipher = nullptr;
    int modulus = 231; // Độ dài alphabet

    while (true) {
        displayMenu();
        int choice = getValidChoice();

        if (choice == 1) {
            if (cipher != nullptr) delete cipher;
            cipher = manualKeyInput(modulus);
        } else if (choice == 2) {
            if (cipher != nullptr) delete cipher;
            cipher = autoKeyGeneration(modulus);
        } else if (choice == 3) {
            encryptText(cipher);
        } else if (choice == 4) {
            decryptText(cipher);
        } else if (choice == 5) {
            wcout << L"\nCảm ơn bạn đã sử dụng chương trình!" << endl;
            break;
        }

        if (choice != 5) {
            wcout << L"\nNhấn Enter để tiếp tục...";
            wcin.get();
            wcout << L"\n" << wstring(60, L'=') << L"\n" << endl;
        }
    }

    if (cipher != nullptr) delete cipher;
    return 0;
}
