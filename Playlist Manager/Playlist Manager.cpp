#include <iostream>
#include <string>
#include <fstream>
using namespace std;

//LOP BAI HAT
class BaiHat {
public:
    string ten, caSi;
    int thoiLuong;
    BaiHat(string t = "", string c = "", int d = 0)
        : ten(t), caSi(c), thoiLuong(d) {
    }
};

//CAY BST LUU THU VIEN
class CayNhac {
private:
    struct Node {
        BaiHat data;
        Node* trai;
        Node* phai;
        Node(BaiHat b) : data(b), trai(nullptr), phai(nullptr) {}
    };

    Node* goc = nullptr;

    Node* chen(Node* n, BaiHat b) {
        if (!n) return new Node(b);
        if (b.ten < n->data.ten) n->trai = chen(n->trai, b);
        else n->phai = chen(n->phai, b);
        return n;
    }

    Node* tim(Node* n, const string& ten) {
        if (!n) return nullptr;
        if (ten == n->data.ten) return n;
        return (ten < n->data.ten) ? tim(n->trai, ten) : tim(n->phai, ten);
    }

    void duyetLNR(Node* n) {
        if (!n) return;
        duyetLNR(n->trai);
        cout << n->data.ten << " - " << n->data.caSi << " - " << n->data.thoiLuong << "s\n";
        duyetLNR(n->phai);
    }

public:
    void themBai(BaiHat b) {
        goc = chen(goc, b);
    }

    BaiHat* timBai(const string& ten) {
        Node* kq = tim(goc, ten);
        return kq ? &kq->data : nullptr;
    }

    void inThuVien() {
        if (!goc) cout << "Thu vien rong.\n";
        else duyetLNR(goc);
    }

    void docFile(const string& tenFile) {
        ifstream f(tenFile);
        if (!f) {
            cout << "Khong mo duoc file!\n";
            return;
        }
        string line;
        while (getline(f, line)) {
            if (line == "") continue;
            size_t p1 = line.find(';');
            size_t p2 = line.find(';', p1 + 1);

            if (p1 == string::npos || p2 == string::npos) continue;
            string ten = line.substr(0, p1);
            string caSi = line.substr(p1 + 1, p2 - p1 - 1);
            int thoiLuong = stoi(line.substr(p2 + 1));

            themBai(BaiHat(ten, caSi, thoiLuong));
        }
        cout << "Da doc du lieu tu file.\n";
        f.close();
    }

    //LƯU FILE 
    Node* getRoot() { return goc; }

    void ghiLNR(Node* n, ofstream& f) {
        if (!n) return;
        ghiLNR(n->trai, f);
        f << n->data.ten << ";" << n->data.caSi << ";" << n->data.thoiLuong << "\n";
        ghiLNR(n->phai, f);
    }
};

//DANH SACH PHAT 
class DanhSachPhat {
private:
    struct NodePL {
        BaiHat* bai;
        NodePL* tiep;
        NodePL* truoc;
        NodePL(BaiHat* b) : bai(b), tiep(this), truoc(this) {}
    };
    NodePL* dau = nullptr;

public:
    void them(BaiHat* b) {
        if (!b) {
            cout << "Khong tim thay bai hat!\n";
            return;
        }
        NodePL* n = new NodePL(b);
        if (!dau) dau = n;
        else {
            n->truoc = dau->truoc;
            n->tiep = dau;
            dau->truoc->tiep = n;
            dau->truoc = n;
        }
        cout << "Da them vao danh sach phat.\n";
    }

    void inDanhSach() {
        if (!dau) {
            cout << "Danh sach phat rong.\n";
            return;
        }
        NodePL* p = dau;
        cout << "=== DANH SACH PHAT ===\n";
        do {
            cout << p->bai->ten << " - " << p->bai->caSi << "\n";
            p = p->tiep;
        } while (p != dau);
    }

    void phat() {
        if (!dau) {
            cout << "Danh sach phat rong.\n";
            return;
        }
        NodePL* hienTai = dau;
        char cmd;
        while (true) {
            cout << "\nDang phat: " << hienTai->bai->ten << " - " << hienTai->bai->caSi;
            cout << "\n(n) ke tiep, (p) truoc do, (q) thoat: ";
            cin >> cmd;

            if (cmd == 'n') hienTai = hienTai->tiep;
            else if (cmd == 'p') hienTai = hienTai->truoc;
            else if (cmd == 'q') break;
        }
    }
};

//UNG DUNG
class UngDung {
private:
    CayNhac thuVien;
    DanhSachPhat playlist;

public:
    void chay() {
        int chon;
        thuVien.docFile("data.txt");

        while (true) {
            cout << "\n=== MENU ===\n";
            cout << "1. Them bai vao thu vien\n";
            cout << "2. Them bai vao danh sach phat\n";
            cout << "3. Tim bai theo ten\n";
            cout << "4. Hien thi thu vien\n";
            cout << "5. Hien thi danh sach phat\n";
            cout << "6. Nghe nhac (next/prev)\n";
            cout << "0. Luu file & Thoat\n";
            cout << "Chon: ";
            cin >> chon;
            cin.ignore();

            if (chon == 1) themBai();
            else if (chon == 2) themPlaylist();
            else if (chon == 3) timBai();
            else if (chon == 4) thuVien.inThuVien();
            else if (chon == 5) playlist.inDanhSach();
            else if (chon == 6) playlist.phat();
            else if (chon == 0) {
                luuFile();
                break;
            }
        }
    }

    void themBai() {
        string t, c;
        int d;
        cout << "Ten bai hat: "; getline(cin, t);
        cout << "Ca si: "; getline(cin, c);
        cout << "Thoi luong (giay): "; cin >> d;
        cin.ignore();
        thuVien.themBai(BaiHat(t, c, d));
        cout << "Da them bai hat.\n";
    }

    void timBai() {
        string t;
        cout << "Nhap ten bai: "; getline(cin, t);
        BaiHat* b = thuVien.timBai(t);
        if (b) cout << b->ten << " - " << b->caSi << "\n";
        else cout << "Khong tim thay.\n";
    }

    void themPlaylist() {
        string t;
        cout << "Ten bai: "; getline(cin, t);
        playlist.them(thuVien.timBai(t));
    }

    //LƯU FILE
    void luuFile() {
        ofstream f("data.txt");
        if (!f) {
            cout << "Khong mo duoc file de ghi!\n";
            return;
        }
        thuVien.ghiLNR(thuVien.getRoot(), f);
        f.close();
        cout << "Da luu du lieu vao data.txt\n";
    }
};

int main() {
    UngDung app;
    app.chay();
    return 0;
}
