#include <bits/stdc++.h>
using namespace std;

// Bảng ký tự 
const vector<string> CHARSET = {
    " ","!","\"","#","$","%","&","'","(",")","*","+",
    ",","-",".","/","0","1","2","3","4","5","6","7",
    "8","9",":",";","<","=",">","?","@",
    "A","B","C","D","E","F","G","H","I","J","K","L",
    "M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z",
    "[","\\","\\\\","]","^","_","`","\n","\t",
    "a","b","c","d","e","f","g","h","i","j","k","l",
    "m","n","o","p","q","r","s","t","u","v","w","x","y","z",
    "{","|","}","~",
    "Ă","Â","Đ","Ê","Ô","Ơ","Ư",
    "Á","À","Ả","Ã","Ạ",
    "Ấ","Ầ","Ẩ","Ẫ","Ậ",
    "Ắ","Ằ","Ẳ","Ẵ","Ặ",
    "É","È","Ẻ","Ẽ","Ẹ",
    "Ế","Ề","Ể","Ễ","Ệ",
    "Í","Ì","Ỉ","Ĩ","Ị",
    "Ó","Ò","Ỏ","Õ","Ọ",
    "Ố","Ồ","Ổ","Ỗ","Ộ",
    "Ớ","Ờ","Ở","Ỡ","Ợ",
    "Ú","Ù","Ủ","Ũ","Ụ",
    "Ứ","Ừ","Ử","Ũ","Ự",
    "Ý","Ỳ","Ỷ","Ỹ","Ỵ",
    "ă","â","đ","ê","ô","ơ","ư",
    "á","à","ả","ã","ạ",
    "ấ","ầ","ẩ","ẫ","ậ",
    "ắ","ằ","ẳ","ẵ","ặ",
    "é","è","ẻ","ẽ","ẹ",
    "ế","ề","ể","ễ","ệ",
    "í","ì","ỉ","ĩ","ị",
    "ó","ò","ỏ","õ","ọ",
    "ố","ồ","ổ","ỗ","ộ",
    "ớ","ờ","ở","ỡ","ợ",
    "ú","ù","ủ","ũ","ụ",
    "ứ","ừ","ử","ữ","ự",
    "ý","ỳ","ỷ","ỹ","ỵ",
     
};
const int CHARSET_SIZE = CHARSET.size(); 

// Hàm tra cứu vị trí ký tự
int indexOf(const string &ch) {
    for (int i = 0; i < CHARSET_SIZE; i++) {
        if (CHARSET[i] == ch) return i;
    }
    return -1;
}

// Hàm tách chuỗi thành token với tương tác người dùng
vector<string> tokenizeInteractive(const string &s) {
    vector<string> tokens;
    for (int i = 0; i < (int)s.size();) {
        bool found = false;
        // Kiểm tra ký tự thoát '\'
        if (s[i] == '\\' && i + 1 < (int)s.size() && (s[i+1] == 'n' || s[i+1] == 't' || s[i+1] == '\\')) {
            char choice;
            while (true) {
                cout << "Ký tự thoát '" << s[i] << s[i+1] << "' được tìm thấy. Xử lý như:\n";
                cout << "  k: Ký tự kép (e.g., \\n là một token, \\ là \\\\)\n";
                cout << "  d: Ký tự đơn (e.g., \\n là hai token '\\' và 'n')\n";
                cout << "Nhập lựa chọn (k/d): ";
                string input;
                getline(cin, input);
                if (input.empty() || (input != "k" && input != "d")) {
                    cout << "Lựa chọn không hợp lệ! Vui lòng nhập 'k' hoặc 'd'.\n";
                    continue;
                }
                choice = input[0];
                break;
            }
            if (choice == 'k') {
                // Xử lý ký tự kép
                if (s[i+1] == 'n') tokens.push_back("\n");
                else if (s[i+1] == 't') tokens.push_back("\t");
                else if (s[i+1] == '\\') tokens.push_back("\\\\"); 
                i += 2; 
            } else {
                // Xử lý ký tự đơn
                tokens.push_back("\\"); 
                if (indexOf(string(1, s[i+1])) != -1) {
                    tokens.push_back(string(1, s[i+1])); 
                } else {
                    cerr << "Ký tự '" << s[i+1] << "' không nằm trong bảng ký tự! Bỏ qua.\n";
                }
                i += 2; 
            }
            found = true;
        } else {
            // Kiểm tra các ký tự khác trong CHARSET
            for (const auto &ch : CHARSET) {
                int len = ch.size();
                if (s.compare(i, len, ch) == 0) {
                    tokens.push_back(ch);
                    i += len;
                    found = true;
                    break;
                }
            }
        }
        if (!found) {
            string ch(1, s[i]);
            if (indexOf(ch) != -1) {
                tokens.push_back(ch);
                i++;
            } else {
                cerr << "Ký tự '" << s[i] << "' không nằm trong bảng ký tự! Bỏ qua.\n";
                i++;
            }
        }
    }
    return tokens;
}

// Hàm hiển thị khóa với ký tự thực tế
void displayKey(const vector<string> &keyTokens) {
    cout << "Khóa: ";
    for (const auto &token : keyTokens) {
        if (token == "\n") {
            cout << "\n"; 
        } else if (token == "\t") {
            cout << "\t"; 
        } else if (token == "\\") {
            cout << "\\"; 
        } else if (token == "\\\\") {
            cout << "\\\\"; 
        } else {
            cout << token;
        }
    }
    cout << endl;
}

// Ghép token thành chuỗi
string joinTokens(const vector<string> &tokens) {
    string res;
    for (auto &t : tokens) res += t;
    return res;
}

// Sinh khóa
vector<string> generateKey(const vector<string> &str, const vector<string> &key) {
    if (key.empty()) return {};
    if (str.empty()) return {};

    vector<string> newKey;
    int n = str.size();
    int m = key.size();

    if (m >= n) {
        // Nếu key dài hơn hoặc bằng plaintext thì chỉ lấy n ký tự đầu
        newKey.assign(key.begin(), key.begin() + n);
    } else {
        // Nếu key ngắn thì lặp lại cho đủ độ dài
        newKey = key;
        for (int i = 0; newKey.size() < n; i++) {
            newKey.push_back(key[i % m]);
        }
    }
    return newKey;
}

// Hàm mã hóa
string cipherText(const vector<string> &str, const vector<string> &key) {
    if (key.empty()) {
        cout << "Chưa có khóa! Hãy nhập hoặc sinh khóa trước.\n";
        return "";
    }
    if (str.empty()) {
        cout << "Chuỗi cần mã hóa rỗng!\n";
        return "";
    }
    vector<string> cipher_tokens;
    cout << "\n=== QUÁ TRÌNH MÃ HÓA (C = (P + K) mod " << CHARSET_SIZE << ") ===\n";
    for (int i = 0; i < (int)str.size(); i++) {
        int pi = indexOf(str[i]);
        int ki = indexOf(key[i]);
        if (pi == -1 || ki == -1) {
            cerr << "Ký tự không hợp lệ!\n";
            return "";
        }
        int ci = (pi + ki) % CHARSET_SIZE;
        cipher_tokens.push_back(CHARSET[ci]);

        // Hiển thị ký tự thực tế từ token với định dạng dễ đọc
        string p_display = (str[i] == "\\") ? "\\" : (str[i] == "\\\\" ? "\\\\" : str[i]);
        string k_display = (key[i] == "\\") ? "\\" : (key[i] == "\\\\" ? "\\\\" : key[i]);
        string c_display = (CHARSET[ci] == "\\") ? "\\" : (CHARSET[ci] == "\\\\" ? "\\\\" : CHARSET[ci]);

        cout << "P[" << i << "]='" << p_display << "'(" << pi << ")"
             << " + K[" << i << "]='" << k_display << "'(" << ki << ")"
             << " = " << (pi + ki) << " mod " << CHARSET_SIZE
             << " = " << ci
             << " => C[" << i << "]='" << c_display << "'\n";
    }
    cout << "====================================================\n";
    return joinTokens(cipher_tokens);
}

// Hàm giải mã
string originalText(const vector<string> &cipher_text, const vector<string> &key) {
    if (key.empty()) {
        cout << "Chưa có khóa! Hãy nhập hoặc sinh khóa trước.\n";
        return "";
    }
    if (cipher_text.empty()) {
        cout << "Chuỗi mã hóa rỗng!\n";
        return "";
    }
    vector<string> orig_tokens;
    cout << "\n=== QUÁ TRÌNH GIẢI MÃ (P = (C - K + N) mod " << CHARSET_SIZE << ") ===\n";
    for (int i = 0; i < (int)cipher_text.size(); i++) {
        int ci = indexOf(cipher_text[i]);
        int ki = indexOf(key[i]);
        if (ci == -1 || ki == -1) {
            cerr << "Ký tự không hợp lệ!\n";
            return "";
        }
        int pi = (ci - ki + CHARSET_SIZE) % CHARSET_SIZE;
        orig_tokens.push_back(CHARSET[pi]);

        // Hiển thị ký tự thực tế từ token với định dạng dễ đọc
        string c_display = (cipher_text[i] == "\\") ? "\\" : (cipher_text[i] == "\\\\" ? "\\\\" : cipher_text[i]);
        string k_display = (key[i] == "\\") ? "\\" : (key[i] == "\\\\" ? "\\\\" : key[i]);
        string p_display = (CHARSET[pi] == "\\") ? "\\" : (CHARSET[pi] == "\\\\" ? "\\\\" : CHARSET[pi]);

        cout << "C[" << i << "]='" << c_display << "'(" << ci << ")"
             << " - K[" << i << "]='" << k_display << "'(" << ki << ")"
             << " = " << (ci - ki + CHARSET_SIZE)
             << " mod " << CHARSET_SIZE
             << " = " << pi
             << " => P[" << i << "]='" << p_display << "'\n";
    }
    cout << "====================================================\n";
    return joinTokens(orig_tokens);
}

void menu() {
    cout << "\n+==============================+\n";
    cout << "|        MÃ HÓA VIGENERE       |\n";
    cout << "| 1. Nhập khóa K thủ công      |\n";
    cout << "| 2. Sinh khóa K tự động       |\n";
    cout << "| 3. Mã Hóa                    |\n";
    cout << "| 4. Giải Mã                   |\n";
    cout << "| 5. Thoát                     |\n";
    cout << "+==============================+\n";
}


string inputMultiLine(const string &prompt) {
    cout << prompt << " (kết thúc bằng dòng chứa 'END'):\n";
    string result, line;
    while (getline(cin, line)) {
        if (line == "END") break;
        result += line + "\n";
    }
    
    if (!result.empty() && result.back() == '\n') {
        result.pop_back();
    }
    return result;
}

int main() {
    string str, keyword, cipher_text;
    vector<string> keyTokens;
    int choice;
    srand(time(0));

    while (true) {
        menu(); 
        while (true) { 
            cout << "Nhập vào lựa chọn (1->5): ";
            string choice_input;
            getline(cin, choice_input);

            // Check for empty input
            if (choice_input.empty()) {
                cout << "Lựa chọn không được rỗng! Hãy nhập số 1-5.\n";
                continue;
            }

            if (isspace(choice_input[0])) {
                cout << "Lựa chọn không được bắt đầu bằng khoảng trắng! Hãy nhập lại số 1-5.\n";
                continue;
            }

            bool isValid = true;
            for (char c : choice_input) {
                if (!isdigit(c)) {
                    isValid = false;
                    break;
                }
            }
            if (isValid) {
                choice = stoi(choice_input); 
                if (choice >= 1 && choice <= 5) {
                    break; 
                }
            }
            cout << "Lựa chọn không hợp lệ! Hãy nhập số 1-5.\n";
        }

        if (choice == 1) {
            while (true) {
                keyword = inputMultiLine("Nhập khóa K");
                if (!keyword.empty()) {
                    keyTokens = tokenizeInteractive(keyword);
                    if (!keyTokens.empty()) {
                        cout << "Khóa đã được nhập thành công. ";
                        displayKey(keyTokens); 
                        break;
                    } else {
                        cout << "Khóa không hợp lệ! Vui lòng nhập lại.\n";
                    }
                } else {
                    cout << "Khóa không được rỗng! Vui lòng nhập lại.\n";
                }
            }
        }
        else if (choice == 2) {
            string len_input;
            int len;
            while (true) {
                cout << "Nhập độ dài khóa: ";
                getline(cin, len_input); 
                bool isValid = true;
                for (char c : len_input) {
                    if (!isdigit(c)) { 
                        isValid = false;
                        break;
                    }
                }
                if (isValid && !len_input.empty()) {
                    len = stoi(len_input); 
                    if (len > 0) {
                        break; 
                    }
                }
                cout << "Độ dài khóa phải là số nguyên dương! Vui lòng nhập lại.\n";
            }
            keyword = "";
            for (int i = 0; i < len; i++) {
                keyword += CHARSET[rand() % CHARSET_SIZE];
            }
            keyTokens = tokenizeInteractive(keyword);
            cout << "Khóa tự động được sinh: ";
            displayKey(keyTokens); 
        }
        else if (choice == 3) {
            while (true) {
                str = inputMultiLine("Nhập chuỗi cần mã hóa");
                if (!str.empty()) {
                    auto strTokens = tokenizeInteractive(str);
                    if (!strTokens.empty()) {
                        auto genKey = generateKey(strTokens, keyTokens);
                        cipher_text = cipherText(strTokens, genKey);
                        if (!cipher_text.empty())
                            cout << "Chuỗi đã mã hóa: " << cipher_text << endl;
                        break;
                    } else {
                        cout << "Chuỗi không hợp lệ! Vui lòng nhập lại.\n";
                    }
                } else {
                    cout << "Chuỗi đầu vào không được rỗng! Vui lòng nhập lại.\n";
                }
            }
        }
        else if (choice == 4) {
            while (true) {
                cipher_text = inputMultiLine("Nhập chuỗi đã mã hóa");
                if (!cipher_text.empty()) {
                    auto cipherTokens = tokenizeInteractive(cipher_text);
                    if (!cipherTokens.empty()) {
                        auto genKey = generateKey(cipherTokens, keyTokens);
                        string orig_text = originalText(cipherTokens, genKey);
                        if (!orig_text.empty())
                            cout << "Chuỗi gốc sau giải mã: " << orig_text << endl;
                        break;
                    } else {
                        cout << "Chuỗi không hợp lệ! Vui lòng nhập lại.\n";
                    }
                } else {
                    cout << "Chuỗi mã hóa không được rỗng! Vui lòng nhập lại.\n";
                }
            }
        }
        else if (choice == 5) {
            cout << "Thoát chương trình...\n";
            break;
        }
    }
    return 0;
}