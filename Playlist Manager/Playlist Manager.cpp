#include <iostream>
#include <string>
#include <fstream>

using namespace std;

//LỚP BÀI HÁT
class BaiHat {
public:
    string ten, caSi;
    int thoiLuong;

    BaiHat(string t = "", string c = "", int d = 0)
        : ten(t), caSi(c), thoiLuong(d) {
    }
};

//CÂY BST LƯU THƯ VIỆN
class CayNhac {
private:
    struct Node {
        BaiHat data;
        Node* trai;
        Node* phai;
        Node(BaiHat b) : data(b), trai(nullptr), phai(nullptr) {}
    };

    Node* goc = nullptr;

    // Thêm bài (BST) theo tên
    Node* chen(Node* n, BaiHat b) {
        if (!n) return new Node(b);
        if (b.ten < n->data.ten) n->trai = chen(n->trai, b);
        else n->phai = chen(n->phai, b);
        return n;
    }

    // Tìm bài theo tên
    Node* tim(Node* n, const string& ten) {
        if (!n) return nullptr;
        if (ten == n->data.ten) return n;
        return (ten < n->data.ten) ? tim(n->trai, ten) : tim(n->phai, ten);
    }

    // Duyệt cây LNR
    void duyetLNR(Node* n) {
        if (!n) return;
        duyetLNR(n->trai);
        cout << n->data.ten << " - " << n->data.caSi << " - " << n->data.thoiLuong << "s\n";
        duyetLNR(n->phai);
    }

    // Tìm node nhỏ nhất (hỗ trợ xóa)
    Node* timMin(Node* n) {
        while (n && n->trai) n = n->trai;
        return n;
    }

    // Xóa node BST
    Node* xoa(Node* n, const string& ten, const string& caSi) {
        if (!n) return nullptr;
        if (ten < n->data.ten) n->trai = xoa(n->trai, ten, caSi);
        else if (ten > n->data.ten) n->phai = xoa(n->phai, ten, caSi);
        else {
            // Kiểm tra cả ca sĩ
            if (caSi != n->data.caSi) {
                n->phai = xoa(n->phai, ten, caSi);
                return n;
            }
            // Node lá hoặc 1 con
            if (!n->trai) { Node* p = n->phai; delete n; return p; }
            if (!n->phai) { Node* p = n->trai; delete n; return p; }
            // Node 2 con
            Node* minRight = timMin(n->phai);
            n->data = minRight->data;
            n->phai = xoa(n->phai, minRight->data.ten, minRight->data.caSi);
        }
        return n;
    }

public:
    // Thêm bài mới
    void themBai(BaiHat b) {
        goc = chen(goc, b);
    }

    // Tìm bài
    BaiHat* timBai(const string& ten) {
        Node* kq = tim(goc, ten);
        return kq ? &kq->data : nullptr;
    }

    // Kiểm tra bài đã tồn tại (trùng tên + ca sĩ)
    bool tonTai(const string& ten, const string& caSi) {
        Node* cur = goc;
        while (cur) {
            if (cur->data.ten == ten && cur->data.caSi == caSi) return true;
            else if (ten < cur->data.ten) cur = cur->trai;
            else cur = cur->phai;
        }
        return false;
    }

    // Xóa bài khỏi thư viện
 void xoaKhoiThuVien() {
     string t, c;
     cout << "Nhap ten bai: "; getline(cin, t);
     cout << "Nhap ca si: "; getline(cin, c);
     thuVien.xoaBai(t, c);
     luuFile(); // 🟢 Ghi lại file ngay sau khi xóa
 }



    // In thư viện
    void inThuVien() {
        if (!goc) cout << "Thu vien rong.\n";
        else duyetLNR(goc);
    }

    // Đọc file
    void docFile(const string& tenFile) {
        ifstream f(tenFile);
        if (!f) { cout << "Khong mo duoc file!\n"; return; }
        string line;
        while (getline(f, line)) {
            if (line.empty()) continue;
            size_t p1 = line.find(';');
            size_t p2 = line.find(';', p1 + 1);
            if (p1 == string::npos || p2 == string::npos) continue;
            string ten = line.substr(0, p1);
            string caSi = line.substr(p1 + 1, p2 - p1 - 1);
            int thoiLuong = stoi(line.substr(p2 + 1));
            themBai(BaiHat(ten, caSi, thoiLuong));
        }
        f.close();
        cout << "Da doc du lieu tu file.\n";
    }

    // Lưu file
    Node* getRoot() { return goc; }
    void ghiLNR(Node* n, ofstream& f) {
        if (!n) return;
        ghiLNR(n->trai, f);
        f << n->data.ten << ";" << n->data.caSi << ";" << n->data.thoiLuong << "\n";
        ghiLNR(n->phai, f);
    }
};

//  DANH SÁCH PHÁT 
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
    // Thêm bài vào playlist
    void them(BaiHat* b) {
        if (!b) { cout << "Khong tim thay bai hat!\n"; return; }
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

    // Xóa bài khỏi playlist
    void xoa(const string& ten, const string& caSi) {
        if (!dau) return;
        NodePL* p = dau;
        do {
            if (p->bai->ten == ten && p->bai->caSi == caSi) {
                if (p->tiep == p) { delete p; dau = nullptr; return; }
                p->truoc->tiep = p->tiep;
                p->tiep->truoc = p->truoc;
                if (p == dau) dau = p->tiep;
                delete p;
                cout << "Da xoa khoi playlist.\n";
                return;
            }
            p = p->tiep;
        } while (p != dau);
        cout << "Khong tim thay trong playlist.\n";
    }

    // In playlist
    void inDanhSach() {
        if (!dau) { cout << "Playlist rong.\n"; return; }
        NodePL* p = dau;
        cout << "=== DANH SÁCH PHÁT ===\n";
        do {
            cout << p->bai->ten << " - " << p->bai->caSi << "\n";
            p = p->tiep;
        } while (p != dau);
    }

    // Nghe nhạc
    void phat() {
        if (!dau) { cout << "Playlist rong.\n"; return; }
        NodePL* hienTai = dau;
        char cmd;
        while (true) {
            cout << "\nDang phat: " << hienTai->bai->ten << " - " << hienTai->bai->caSi;
            cout << "\n(n) next, (p) prev, (q) quit: ";
            cin >> cmd;
            if (cmd == 'n') hienTai = hienTai->tiep;
            else if (cmd == 'p') hienTai = hienTai->truoc;
            else if (cmd == 'q') break;
        }
    }
};

// ỨNG DỤNG 
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
            cout << "7. Xoa bai khoi thu vien\n";
            cout << "8. Xoa bai khoi playlist\n";
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
            else if (chon == 7) xoaKhoiThuVien();
            else if (chon == 8) xoaKhoiPlaylist();
            else if (chon == 0) { luuFile(); break; }
        }
    }

    // Thêm bài vào thư viện, kiểm tra trùng tên + ca sĩ
    void themBai() {
        string t, c;
        int d;
        cout << "Ten bai hat: "; getline(cin, t);
        cout << "Ca si: "; getline(cin, c);

        // Kiểm tra trùng
        if (thuVien.tonTai(t, c)) {
            cout << "Bai hat da ton tai trong thu vien!\n";
            return;
        }

        cout << "Thoi luong (giay): "; cin >> d;
        cin.ignore();
        thuVien.themBai(BaiHat(t, c, d));
        cout << "Da them bai hat.\n";
    }

    // Tìm bài
    void timBai() {
        string t;
        cout << "Nhap ten bai: "; getline(cin, t);
        BaiHat* b = thuVien.timBai(t);
        if (b) cout << b->ten << " - " << b->caSi << "\n";
        else cout << "Khong tim thay.\n";
    }

    // Thêm bài vào playlist
    void themPlaylist() {
        string t, c;
        cout << "Ten bai: "; getline(cin, t);
        cout << "Ca si: "; getline(cin, c);
        BaiHat* b = thuVien.timBai(t);
        if (b && b->caSi == c)
            playlist.them(b);
        else
            cout << "Khong tim thay bai hat trong thu vien.\n";
    }

    // Xóa bài khỏi thư viện
    void xoaKhoiThuVien() {
        string t, c;
        cout << "Nhap ten bai: "; getline(cin, t);
        cout << "Nhap ca si: "; getline(cin, c);
        thuVien.xoaBai(t, c);
    }

    // Xóa bài khỏi playlist
    void xoaKhoiPlaylist() {
        string t, c;
        cout << "Nhap ten bai: "; getline(cin, t);
        cout << "Nhap ca si: "; getline(cin, c);
        playlist.xoa(t, c);
    }

    // Lưu file
    void luuFile() {
        ofstream f("data.txt");
        if (!f) { cout << "Khong mo duoc file de ghi!\n"; return; }
        thuVien.ghiLNR(thuVien.getRoot(), f);
        f.close();
        cout << "Da luu du lieu vao data.txt\n";
    }
};

// MAIN 
int main() {
    UngDung app;
    app.chay();
    return 0;
}
