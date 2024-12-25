#include <iostream>
#include <string>
#include <unordered_map>
#include <cctype>
#include <stack>

using namespace std;

// Ham thuc hien phep toan PHU DINH (~)
bool PhuDinh(bool giaTri) {
    return !giaTri;
}

// Ham thuc hien phep toan VA (&)
bool Va(bool a, bool b) {
    return a && b;
}

// Ham thuc hien phep toan HOAC (|)
bool Hoac(bool a, bool b) {
    return a || b;
}

// Ham thuc hien phep toan SAI LECH (>)
bool SaiLech(bool a, bool b) {
    return !a || b;
}

// Ham kiem tra tinh hop le cua bieu thuc logic
bool KiemTraHopLe(const string& bieuThuc) {
    int ngoacMo = 0;

    // Kiem tra tung ky tu trong bieu thuc
    for (size_t i = 0; i < bieuThuc.size(); i++) {
        char kyTu = bieuThuc[i];

        // Kiem tra dau ngoac
        if (kyTu == '(') {
            ngoacMo++;
        } else if (kyTu == ')') {
            if (ngoacMo == 0) return false; // Co ngoac dong ma khong co ngoac mo
            ngoacMo--;
        }

        // Kiem tra cac toan tu hop le
        if (kyTu == '&' || kyTu == '|' || kyTu == '>' || kyTu == '~') {
            if (i == 0 || i == bieuThuc.size() - 1) {
                return false; // Khong co toan tu o dau hoac cuoi bieu thuc
            }
            if (bieuThuc[i - 1] == '(' || bieuThuc[i + 1] == ')') {
                return false; // Khong co toan tu li?n sau dau ngoac
            }
        }
    }

    // Dam bao rang cac dau ngoac mo va dong deu khop
    return ngoacMo == 0;
}

// Ham tinh toan gia tri cua bieu thuc logic
bool TinhGiaTriBieuThuc(const string& bieuThuc, const unordered_map<char, bool>& giaTriBien) {
    stack<bool> nganXepToanHang; // Ngan xep luu tru cac gia tri logic
    stack<char> nganXepToanTu; // Ngan xep luu tru cac toan tu logic

    for (size_t i = 0; i < bieuThuc.size(); i++) {
        char kyTu = bieuThuc[i];

        // Neu la bien (A-Z), lay gia tri tu bang gia tri
        if (isalpha(kyTu)) {
            nganXepToanHang.push(giaTriBien.at(kyTu));
        }
        // Neu gap toan tu phu dinh (~), , lay gia tri tu bang gia tri
        else if (kyTu == '~') {
            bool toanHang = nganXepToanHang.top();
            nganXepToanHang.pop();
            nganXepToanHang.push(PhuDinh(toanHang));
        }
        // Neu gap toan tu VA(&)
        else if (kyTu == '&') {
            nganXepToanTu.push(kyTu);
        }
        // Neu gap toan tu HOAC (|)
        else if (kyTu == '|') {
            nganXepToanTu.push(kyTu);
        }
        // Neu gap toan tu SAI LECH (>)
        else if (kyTu == '>') {
            nganXepToanTu.push(kyTu);
        }
        // Neu gap dau ngoac mo, thuc hien phep toan trong ngoac
        else if (kyTu == '(') {
            nganXepToanTu.push(kyTu);
        }
        // Neu gap dau ngoac dong, thuc hien phep toan trong ngoac
        else if (kyTu == ')') {
            while (nganXepToanTu.top() != '(') {
                char toanTu = nganXepToanTu.top();
                nganXepToanTu.pop();
                bool toanHangPhai = nganXepToanHang.top();
                nganXepToanHang.pop();
                bool toanHangTrai = nganXepToanHang.top();
                nganXepToanHang.pop();

                // Thuc hien phep toan logic tuong ung
                if (toanTu == '&') {
                    nganXepToanHang.push(Va(toanHangTrai, toanHangPhai));
                } else if (toanTu == '|') {
                    nganXepToanHang.push(Hoac(toanHangTrai, toanHangPhai));
                } else if (toanTu == '>') {
                    nganXepToanHang.push(SaiLech(toanHangTrai, toanHangPhai));
                }
            }
            nganXepToanTu.pop(); // Xoa dau ngoac mo
        }
    }

    // Xu ly cac phep toan con lai trong ngan xep toan tu
    while (!nganXepToanTu.empty()) {
        char toanTu = nganXepToanTu.top();
        nganXepToanTu.pop();
        bool toanHangPhai = nganXepToanHang.top();
        nganXepToanHang.pop();
        bool toanHangTrai = nganXepToanHang.top();
        nganXepToanHang.pop();

        if (toanTu == '&') {
            nganXepToanHang.push(Va(toanHangTrai, toanHangPhai));
        } else if (toanTu == '|') {
            nganXepToanHang.push(Hoac(toanHangTrai, toanHangPhai));
        } else if (toanTu == '>') {
            nganXepToanHang.push(SaiLech(toanHangTrai, toanHangPhai));
        }
    }

    return nganXepToanHang.top(); // Tro ve gia tri cuoi cung
}

int main() {
    string bieuThuc;
    cout << "Nhap bieu thuc logic: ";
    getline(cin, bieuThuc);

    // Kiem tra tinh hop le cua bieu thuc
    if (!KiemTraHopLe(bieuThuc)) {
        cout << "Bieu thuc khong hop le: " << endl;
        return 1;
    }

    // Nhap gia tri cua cac bien logic
    unordered_map<char, bool> giaTriBien;
    cout << "Nhap gia tri cho cac bien (vi du: A: true, B: false):" << endl;

    for (size_t i = 0; i < bieuThuc.size(); i++) {
        if (isalpha(bieuThuc[i])) {
            char bien = bieuThuc[i];
            if (giaTriBien.find(bien) == giaTriBien.end()) {
                bool giaTri;
                cout << "Nhap gia tri cho " << bien << " (true hoac false): ";
                cin >> boolalpha >> giaTri;
                giaTriBien[bien] = giaTri;
            }
        }
    }

    // Tinh toan gia tri cua bieu thuc
    bool ketQua = TinhGiaTriBieuThuc(bieuThuc, giaTriBien);
    cout << "Ket qua cua bieu thuc loi: " << (ketQua ? "True" : "False") << endl;

    return 0;
}


