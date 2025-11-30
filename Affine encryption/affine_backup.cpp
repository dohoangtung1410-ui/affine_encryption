#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <random>
#include <limits>
#include <windows.h>
#include <io.h>
#include <fcntl.h>
using namespace std;

// Bảng ký tự alphabet
const vector<wchar_t> ALPHABET = {
    // Latin cơ bản
    L'a',L'b',L'c',L'd',L'e',L'f',L'g',L'h',L'i',L'j',L'k',L'l',L'm',L'n',
    L'o',L'p',L'q',L'r',L's',L't',L'u',L'v',L'w',L'x',L'y',L'z',L'A',L'B',
    L'C',L'D',L'E',L'F',L'G',L'H',L'I',L'J',L'K',L'L',L'M',L'N',L'O',L'P',
    L'Q',L'R',L'S',L'T',L'U',L'V',L'W',L'X',L'Y',L'Z',L' ',L'~',L'`',L'!',
    L'@',L'#',L'$',L'%',L'^',L'&',L'*',L'(',L')',L'-',L'_',L'+',L'=',L'|',
    L'\\',L'"',L'\n',L'\t',L'{',L'[',L']',L'}',L':',L';',L'\'',L'<',L',',
    L'.',L'>',L'?',L'/',L'0',L'1',L'2',L'3',L'4',L'5',L'6',L'7',L'8',L'9',
    // Tiếng Việt - chữ thường
    L'à',L'á',L'ả',L'ã',L'ạ',L'ă',L'ằ',L'ắ',L'ẳ',L'ẵ',L'ặ',L'â',L'ầ',L'ấ',L'ẩ',L'ẫ',L'ậ',
    L'è',L'é',L'ẻ',L'ẽ',L'ẹ',L'ê',L'ề',L'ế',L'ể',L'ễ',L'ệ',
    L'ì',L'í',L'ỉ',L'ĩ',L'ị',
    L'ò',L'ó',L'ỏ',L'õ',L'ọ',L'ô',L'ồ',L'ố',L'ổ',L'ỗ',L'ộ',L'ơ',L'ờ',L'ớ',L'ở',L'ỡ',L'ợ',
    L'ù',L'ú',L'ủ',L'ũ',L'ụ',L'ư',L'ừ',L'ứ',L'ử',L'ữ',L'ự',
    L'ỳ',L'ý',L'ỷ',L'ỹ',L'ỵ',
    L'đ',
    // Tiếng Việt - chữ hoa
    L'À',L'Á',L'Ả',L'Ã',L'Ạ',L'Ă',L'Ằ',L'Ắ',L'Ẳ',L'Ẵ',L'Ặ',L'Â',L'Ầ',L'Ấ',L'Ẩ',L'Ẫ',L'Ậ',
    L'È',L'É',L'Ẻ',L'Ẽ',L'Ẹ',L'Ê',L'Ề',L'Ế',L'Ể',L'Ễ',L'Ệ',
    L'Ì',L'Í',L'Ỉ',L'Ĩ',L'Ị',
    L'Ò',L'Ó',L'Ỏ',L'Õ',L'Ọ',L'Ô',L'Ồ',L'Ố',L'Ổ',L'Ỗ',L'Ộ',L'Ơ',L'Ờ',L'Ớ',L'Ở',L'Ỡ',L'Ợ',
    L'Ù',L'Ú',L'Ủ',L'Ũ',L'Ụ',L'Ư',L'Ừ',L'Ứ',L'Ử',L'Ữ',L'Ự',
    L'Ỳ',L'Ý',L'Ỷ',L'Ỹ',L'Ỵ',
    L'Đ'
};

class AffineCipher {
private:
    map<wchar_t, int> index_of;
    map<int, wchar_t> char_of;
    int modulus;
    
    long long gcd(long long a, long long b) {
        while (b) {
            long long temp = b;
            b = a % b;
            a = temp;
        }
        return a;
    }
    
    long long extended_gcd(long long a, long long b, long long& x, long long& y) {
        if (a == 0) {
            x = 0;
            y = 1;
            return b;
        }
        long long x1, y1;
        long long gcd_val = extended_gcd(b % a, a, x1, y1);
        x = y1 - (b / a) * x1;
        y = x1;
        return gcd_val;
    }
    
    long long mod_inverse(long long a, long long m) {
        long long x, y;
        long long g = extended_gcd(a, m, x, y);
        if (g != 1) return -1;
        return ((x % m) + m) % m;
    }
    
    bool is_valid_key(long long a) {
        return gcd(a, modulus) == 1;
    }

public:
    long long a_original, b_original;
    long long a, b;
    long long a_inverse;
    
    AffineCipher(long long a_val, long long b_val) {
        modulus = ALPHABET.size();
        
        for (size_t i = 0; i < ALPHABET.size(); i++) {
            index_of[ALPHABET[i]] = i;
            char_of[i] = ALPHABET[i];
        }
        
        a_original = a_val;
        b_original = b_val;
        
        a = ((a_val % modulus) + modulus) % modulus;
        b = ((b_val % modulus) + modulus) % modulus;
        
        if (!is_valid_key(a)) {
            throw runtime_error("Khoa khong hop le! a phai nguyen to cung nhau voi " + to_string(modulus));
        }
        
        a_inverse = mod_inverse(a, modulus);
    }
    
    wstring encrypt(const wstring& plaintext, bool verbose = true) {
        wstring ciphertext = L"";
        for (size_t i = 0; i < plaintext.length(); i++) {
            wchar_t ch = plaintext[i];
            if (index_of.find(ch) != index_of.end()) {
                int x = index_of[ch];
                int y = (a * x + b) % modulus;
                wchar_t new_char = char_of[y];
                if (verbose) {
                    wcout << L"[" << i << L"] ";
                    wcout << L"'";
                    WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), &ch, 1, NULL, NULL);
                    wcout << L"' -> chi so " << x << L" => y=" << y << L" -> '";
                    WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), &new_char, 1, NULL, NULL);
                    wcout << L"'" << endl;
                }
                ciphertext += new_char;
            } else {
                if (verbose) {
                    wcout << L"[" << i << L"] Ky tu khong ho tro: '";
                    WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), &ch, 1, NULL, NULL);
                    wcout << L"' (giu nguyen)" << endl;
                }
                ciphertext += ch;
            }
        }
        return ciphertext;
    }
    
    wstring decrypt(const wstring& ciphertext, bool verbose = true) {
        wstring plaintext = L"";
        for (size_t i = 0; i < ciphertext.length(); i++) {
            wchar_t ch = ciphertext[i];
            if (index_of.find(ch) != index_of.end()) {
                int y = index_of[ch];
                long long temp = (y - b);
                temp = ((temp % modulus) + modulus) % modulus;
                int x = (a_inverse * temp) % modulus;
                wchar_t new_char = char_of[x];
                if (verbose) {
                    wcout << L"[" << i << L"] ";
                    wcout << L"'";
                    WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), &ch, 1, NULL, NULL);
                    wcout << L"' -> chi so " << y << L" => x=" << x << L" -> '";
                    WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), &new_char, 1, NULL, NULL);
                    wcout << L"'" << endl;
                }
                plaintext += new_char;
            } else {
                if (verbose) {
                    wcout << L"[" << i << L"] Ky tu khong ho tro: '";
                    WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), &ch, 1, NULL, NULL);
                    wcout << L"' (giu nguyen)" << endl;
                }
                plaintext += ch;
            }
        }
        return plaintext;
    }
};

void display_menu() {
    wcout << L"+" << wstring(50, L'=') << L"+" << endl;
    wcout << L"|" << wstring(15, L' ') << L"MA HOA AFFINE" << wstring(16, L' ') << L"|" << endl;
    wcout << L"+" << wstring(50, L'=') << L"+" << endl;
    wcout << L"| 1. Nhap khoa (a,b) thu cong               |" << endl;
    wcout << L"| 2. Sinh khoa (a,b) tu dong                |" << endl;
    wcout << L"| 3. Ma hoa                                 |" << endl;
    wcout << L"| 4. Giai ma                                |" << endl;
    wcout << L"| 5. Thoat                                  |" << endl;
    wcout << L"+" << wstring(50, L'=') << L"+" << endl;
}

int get_valid_choice() {
    int choice;
    while (true) {
        wcout << L"Nhap lua chon (1-5): ";
        if (wcin >> choice) {
            if (choice >= 1 && choice <= 5) {
                wcin.ignore(numeric_limits<streamsize>::max(), L'\n');
                return choice;
            } else {
                wcout << L"Lua chon phai tu 1 den 5, vui long nhap lai!" << endl;
            }
        } else {
            wcout << L"Lua chon khong hop le, vui long nhap lai!" << endl;
            wcin.clear();
            wcin.ignore(numeric_limits<streamsize>::max(), L'\n');
        }
    }
}

long long gcd_helper(long long a, long long b) {
    while (b) {
        long long temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

AffineCipher* manual_key_input() {
    wcout << L"\n--- NHAP KHOA THU CONG ---" << endl;
    wcout << L"(Nhap 'q' hoac 'quit' bat ky luc nao de quay lai menu)" << endl;
    
    int modulus = ALPHABET.size();
    wcout << L"Do dai bang ky tu (modulo) = " << modulus << endl;
    
    vector<int> valid_a_samples;
    for (int i = 1; i < modulus; i++) {
        if (gcd_helper(i, modulus) == 1) {
            valid_a_samples.push_back(i);
        }
    }
    
    int preview_count = min(20, (int)valid_a_samples.size());
    wcout << L"- a phai NGUYEN TO CUNG NHAU voi modulo (gcd(a, modulo) = 1)" << endl;
    wcout << L"  Vi du a hop le: ";
    for (int i = 0; i < preview_count; i++) {
        wcout << valid_a_samples[i];
        if (i < preview_count - 1) wcout << L", ";
    }
    if (valid_a_samples.size() > (size_t)preview_count) wcout << L" ...";
    wcout << endl;
    
    wcout << L"- b co the la BAT KY so nguyen (ke ca so am)" << endl;
    wcout << L"  So am se duoc tu dong chuan hoa ve [0.." << modulus-1 << L"]" << endl;
    wcout << L"  Vi du: b=-10 se thanh b=" << ((-10 % modulus) + modulus) % modulus << endl;
    
    long long a, b;
    wstring input;
    
    // Nhập a
    while (true) {
        wcout << L"\nNhap gia tri a (co the am, nhung sau chuan hoa khong duoc chia het cho 3,7,11): ";
        wcin >> input;
        
        if (input == L"q" || input == L"quit") {
            wcout << L"Huy nhap khoa, quay lai menu..." << endl;
            wcin.ignore(numeric_limits<streamsize>::max(), L'\n');
            return nullptr;
        }
        
        try {
            a = stoll(input);
            long long a_norm = ((a % modulus) + modulus) % modulus;
            wcout << L"  -> a sau chuan hoa: " << a_norm << endl;
            
            if (a_norm % 3 == 0 || a_norm % 7 == 0 || a_norm % 11 == 0) {
                wcout << L"  X a=" << a_norm << L" chia het cho 3, 7 hoac 11. Vui long nhap lai!" << endl;
                continue;
            }
            
            if (gcd_helper(a_norm, modulus) != 1) {
                wcout << L"  X gcd(" << a_norm << L", " << modulus << L") != 1. Vui long nhap lai!" << endl;
                continue;
            }
            
            wcout << L"  V a=" << a_norm << L" hop le!" << endl;
            break;
        } catch (...) {
            wcout << L"Gia tri a phai la so nguyen. Vui long nhap lai!" << endl;
        }
    }
    
    // Nhập b
    while (true) {
        wcout << L"\nNhap gia tri b (bat ky so nguyen, ke ca so am): ";
        wcin >> input;
        
        if (input == L"q" || input == L"quit") {
            wcout << L"Huy nhap khoa, quay lai menu..." << endl;
            wcin.ignore(numeric_limits<streamsize>::max(), L'\n');
            return nullptr;
        }
        
        try {
            b = stoll(input);
            long long b_norm = ((b % modulus) + modulus) % modulus;
            wcout << L"  -> b sau chuan hoa: " << b_norm << endl;
            wcout << L"  V b=" << b_norm << L" hop le!" << endl;
            break;
        } catch (...) {
            wcout << L"Gia tri b phai la so nguyen. Vui long nhap lai!" << endl;
        }
    }
    
    wcin.ignore(numeric_limits<streamsize>::max(), L'\n');
    
    try {
        AffineCipher* cipher = new AffineCipher(a, b);
        wcout << L"\n" << wstring(50, L'=') << endl;
        wcout << L"Khoa da nhap: a = " << cipher->a_original << L", b = " << cipher->b_original << endl;
        wcout << L"Khoa sau chuan hoa: a = " << cipher->a << L", b = " << cipher->b << endl;
        wcout << L"Nghich dao cua " << cipher->a << L" mod " << modulus << L" = " << cipher->a_inverse << endl;
        wcout << wstring(50, L'=') << endl;
        return cipher;
    } catch (const exception& e) {
        wcout << L"Loi khi tao khoa!" << endl;
        return nullptr;
    }
}

AffineCipher* auto_key_generation() {
    wcout << L"\n--- SINH KHOA TU DONG ---" << endl;
    wcout << L"(Nhap 'q' hoac 'quit' de quay lai menu)" << endl;
    wcout << L"Nhan Enter de sinh khoa tu dong, hoac 'q' de huy: ";
    
    wstring confirm;
    getline(wcin, confirm);
    
    if (confirm == L"q" || confirm == L"quit") {
        wcout << L"Huy sinh khoa, quay lai menu..." << endl;
        return nullptr;
    }
    
    random_device rd;
    mt19937_64 gen(rd());
    uniform_int_distribution<long long> dis(-100000000, 100000000);
    
    int modulus = ALPHABET.size();
    
    while (true) {
        long long a = dis(gen);
        long long b = dis(gen);
        
        long long a_norm = ((a % modulus) + modulus) % modulus;
        
        if (gcd_helper(a_norm, modulus) == 1) {
            try {
                AffineCipher* cipher = new AffineCipher(a, b);
                wcout << L"\nKhoa duoc sinh (goc): a = " << a << L", b = " << b << endl;
                wcout << L"Khoa sau chuan hoa: a = " << cipher->a << L", b = " << cipher->b << endl;
                wcout << L"Nghich dao cua " << cipher->a << L" mod " << modulus << L" = " << cipher->a_inverse << endl;
                return cipher;
            } catch (...) {
                continue;
            }
        }
    }
}

void encrypt_text(AffineCipher* cipher) {
    if (cipher == nullptr) {
        wcout << L"Chua co khoa! Vui long chon chuc nang 1 hoac 2 truoc." << endl;
        return;
    }
    
    wcout << L"\n--- MA HOA ---" << endl;
    wcout << L"(Nhap 'q' hoac 'quit' de quay lai menu)" << endl;
    wcout << L"Nhap van ban can ma hoa: ";
    
    wstring text;
    getline(wcin, text);
    
    if (text == L"q" || text == L"quit") {
        wcout << L"Huy ma hoa, quay lai menu..." << endl;
        return;
    }
    
    wcout << L"\nChi tiet tung ky tu:" << endl;
    wstring encrypted = cipher->encrypt(text, true);
    wcout << L"\nKet qua ma hoa: ";
    WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), encrypted.c_str(), encrypted.length(), NULL, NULL);
    wcout << endl;
}

void decrypt_text(AffineCipher* cipher) {
    if (cipher == nullptr) {
        wcout << L"Chua co khoa! Vui long chon chuc nang 1 hoac 2 truoc." << endl;
        return;
    }
    
    wcout << L"\n--- GIAI MA ---" << endl;
    wcout << L"(Nhap 'q' hoac 'quit' de quay lai menu)" << endl;
    wcout << L"Nhap van ban can giai ma: ";
    
    wstring text;
    getline(wcin, text);
    
    if (text == L"q" || text == L"quit") {
        wcout << L"Huy giai ma, quay lai menu..." << endl;
        return;
    }
    
    wcout << L"\nChi tiet tung ky tu:" << endl;
    wstring decrypted = cipher->decrypt(text, true);
    wcout << L"\nKet qua giai ma: ";
    WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), decrypted.c_str(), decrypted.length(), NULL, NULL);
    wcout << endl;
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stdin), _O_U16TEXT);
    
    AffineCipher* cipher = nullptr;
    
    while (true) {
        display_menu();
        int choice = get_valid_choice();
        
        if (choice == 1) {
            AffineCipher* result = manual_key_input();
            if (result != nullptr) {
                if (cipher != nullptr) delete cipher;
                cipher = result;
            }
        } else if (choice == 2) {
            AffineCipher* result = auto_key_generation();
            if (result != nullptr) {
                if (cipher != nullptr) delete cipher;
                cipher = result;
            }
        } else if (choice == 3) {
            encrypt_text(cipher);
        } else if (choice == 4) {
            decrypt_text(cipher);
        } else if (choice == 5) {
            wcout << L"\nCam on ban da su dung chuong trinh!" << endl;
            break;
        }
        
        if (choice != 5) {
            wcout << L"\nNhan Enter de tiep tuc...";
            wcin.get();
            wcout << L"\n" << wstring(60, L'=') << L"\n" << endl;
        }
    }
    
    if (cipher != nullptr) delete cipher;
    
    return 0;
}