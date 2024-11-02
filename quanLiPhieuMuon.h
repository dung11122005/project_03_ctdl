#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <queue>
#include <regex>
#include "quanLiSach.h"


using namespace std;


#define RESET "\033[0m"
#define YELLOW "\033[1;33m"
#define GREEN "\033[0;32m"
#define GREENS "\033[38;5;82m"
#define BLUE "\033[34m"
#define PINK "\033[38;5;213m"
#define RED "\033[0;31m"

class BanDoc {
private:
	string maBanDoc;
	string hoTen;
	string ngayDangKy;

public:
	BanDoc(string ma, string ten, string ngayDK) {
		maBanDoc = ma;
		hoTen = ten;
		ngayDangKy = ngayDK;
	}

	void hienThiThongTin() {
		cout << "Ma Ban Doc: " << maBanDoc << endl;
		cout << "Ho Ten: " << hoTen << endl;
		cout << "Ngay Dang Ky: " << ngayDangKy << endl;
	}

	string getMaBanDoc() { return maBanDoc; }
};


class PhieuMuon {
private:
	int soPhieuMuon;
	string maBanDoc;
	string maSach;
	string ngayMuon;
	string ngayPhaiTra;
	int tinhTrang; // 1 là đang mượn, 0 là đã trả

public:

	PhieuMuon()
		: soPhieuMuon(0), maBanDoc(""), maSach(""), ngayMuon(""), ngayPhaiTra(""), tinhTrang(0) {}

	PhieuMuon(int soPhieu, string maBD, string maS, string ngayM, string ngayTra, int tinhtrang) {
		soPhieuMuon = soPhieu;
		maBanDoc = maBD;
		maSach = maS;
		ngayMuon = ngayM;
		ngayPhaiTra = ngayTra;
		tinhTrang = tinhtrang;
	}

	int getSoPhieuMuon() {
		return soPhieuMuon;
	}

	string getMaBanDoc() {
		return maBanDoc;
	}

	string getMaSach() {
		return maSach;
	}

	string getNgayMuon() {
		return ngayMuon;
	}

	string getNgayPhaiTra() {
		return ngayPhaiTra;
	}

	int getTinhTrang() {
		return tinhTrang;
	}

	// Hàm setter cho tình trạng
	void setTinhTrang(int tt) {
		this->tinhTrang = tt;
	}


};

// Cấu trúc node của cây nhị phân tìm kiếm
struct TreeNode {
	PhieuMuon phieu;
	TreeNode* left;
	TreeNode* right;

	TreeNode(const PhieuMuon& p) : phieu(p), left(nullptr), right(nullptr) {}
};

// Hàm kiểm tra định dạng mã (chỉ chứa chữ cái và chữ số, độ dài từ 3 đến 10)
bool isValidMa(const string& ma) {
	// Kiểm tra chuỗi không rỗng hoặc chỉ chứa dấu cách
	if (ma.empty() || ma.find_first_not_of(' ') == string::npos) return false;
	return regex_match(ma, regex("^[A-Za-z0-9]{3,15}$"));
}

// Hàm kiểm tra định dạng ngày theo dạng dd/mm/yyyy
bool isValidDate(const string& date) {
	// Kiểm tra chuỗi không rỗng hoặc chỉ chứa dấu cách
	if (date.empty() || date.find_first_not_of(' ') == string::npos) return false;
	if (!regex_match(date, regex("^\\d{2}/\\d{2}/\\d{4}$"))) return false;

	// Tách chuỗi ngày, tháng, năm để kiểm tra tính hợp lệ
	int day = stoi(date.substr(0, 2));
	int month = stoi(date.substr(3, 2));
	int year = stoi(date.substr(6, 4));

	if (month < 1 || month > 12 || day < 1 || day > 31) return false;

	// Kiểm tra số ngày hợp lệ trong từng tháng
	if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30) return false;
	if (month == 2) {
		bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
		if (day > (isLeap ? 29 : 28)) return false;
	}
	return true;
}

// Kiểm tra năm nhuận
bool isLeapYear(int year) {
	return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

// Lấy số ngày trong tháng (có tính năm nhuận)
int daysInMonth(int month, int year) {
	switch (month) {
	case 1: case 3: case 5: case 7: case 8: case 10: case 12: return 31;
	case 4: case 6: case 9: case 11: return 30;
	case 2: return isLeapYear(year) ? 29 : 28;
	default: return 30; // Tháng không hợp lệ
	}
}

// Hàm tách chuỗi ngày thành vector<int> [ngày, tháng, năm]
vector<int> parseDate(const string& date) {
	vector<int> result;
	stringstream ss(date);
	string temp;

	while (getline(ss, temp, '/')) {
		result.push_back(stoi(temp)); // chuyển từng phần của ngày thành số nguyên
	}

	return result;
}

// Hàm tính ngày phải trả
string tinhNgayPhaiTra(string ngayMuon, int soNgayMuon) {
	// Tách ngày, tháng, năm từ chuỗi
	vector<int> date = parseDate(ngayMuon);
	int day = date[0];
	int month = date[1];
	int year = date[2];

	// Cộng số ngày mượn
	day += soNgayMuon;

	// Điều chỉnh ngày và tháng khi vượt quá số ngày trong tháng hiện tại
	while (day > daysInMonth(month, year)) {
		day -= daysInMonth(month, year); // Trừ đi số ngày trong tháng hiện tại
		month++; // Chuyển sang tháng tiếp theo

		// Nếu tháng vượt quá 12, tăng năm và đặt tháng thành 1
		if (month > 12) {
			month = 1;
			year++;
		}
	}

	// Trả về chuỗi ngày phải trả theo định dạng "dd/mm/yyyy"
	return to_string(day) + "/" + to_string(month) + "/" + to_string(year);
}

class QuanLyPhieuMuon {
private:
	vector<PhieuMuon> danhSachPhieuMuon;
	vector<BanDoc> danhSachBanDoc; // Danh sách bạn đọc

	// Hàm chèn phiếu mượn vào cây nhị phân tìm kiếm
	TreeNode* insert(TreeNode* root, PhieuMuon phieu) {
		if (!root) {
			return new TreeNode(phieu);
		}
		if (phieu.getSoPhieuMuon() < root->phieu.getSoPhieuMuon()) {
			root->left = insert(root->left, phieu);
		}
		else {
			root->right = insert(root->right, phieu);
		}
		return root;
	}

	// Hàm duyệt cây theo thứ tự in-order để lấy danh sách đã sắp xếp
	void LNR(TreeNode* root, queue<PhieuMuon>& sortedQueue) {
		if (!root) return;
		LNR(root->left, sortedQueue);
		sortedQueue.push(root->phieu); // Thêm vào hàng đợi
		LNR(root->right, sortedQueue);
	}

public:
	vector<PhieuMuon>& getDanhSachPhieuMuon() {
		return danhSachPhieuMuon;
	}

	// Setter cho danh sách phiếu mượn
	void setDanhSachPhieuMuon(const vector<PhieuMuon>& dsPhieuMuon) {
		danhSachPhieuMuon = dsPhieuMuon;
	}

	// Getter cho danh sách bạn đọc
	vector<BanDoc>& getDanhSachBanDoc() {
		return danhSachBanDoc;
	}

	// Setter cho danh sách bạn đọc
	void setDanhSachBanDoc(const vector<BanDoc>& dsBanDoc) {
		danhSachBanDoc = dsBanDoc;
	}

	void themBanDoc(BanDoc& banDoc) {
		danhSachBanDoc.push_back(banDoc);
	}

	void hienThiPhieuMuon() {
		// Bước 1: Chèn tất cả phiếu mượn vào cây nhị phân
		TreeNode* root = nullptr;
		for (const auto& phieu : danhSachPhieuMuon) {
			root = insert(root, phieu);
		}

		// Bước 2: Duyệt cây và lưu các phiếu mượn đã sắp xếp vào hàng đợi
		queue<PhieuMuon> sortedQueue;
		LNR(root, sortedQueue);

		// In tiêu đề của bảng
		cout << setw(10) << GREENS << "So Phieu"
			<< setw(15) << "Ma Ban Doc"
			<< setw(15) << "Ma Sach"
			<< setw(15) << "Ngay Muon"
			<< setw(20) << "Ngay Phai Tra"
			<< setw(15) << "Tinh Trang"
			<< endl;

		cout << "-------------------------------------------------------------------------------------------" << YELLOW << endl;

		// Duyệt qua danh sách phiếu mượn và in từng hàng
		while (!sortedQueue.empty()) {
			PhieuMuon phieu = sortedQueue.front();
			sortedQueue.pop();
			cout << left << setw(13) << phieu.getSoPhieuMuon()
				<< setw(18) << phieu.getMaBanDoc()
				<< setw(13) << phieu.getMaSach()
				<< setw(16) << phieu.getNgayMuon()
				<< setw(18) << phieu.getNgayPhaiTra()
				<< setw(15) << (phieu.getTinhTrang() == 1 ? BLUE"Dang Muon" : PINK"Da Tra")
				<< YELLOW << endl;
		}
	}
	// Hàm đọc toàn bộ nội dung file Phieumuon.txt và lưu vào vector
	vector<PhieuMuon> docDanhSachPhieuMuon(const string& fileName) {
		vector<PhieuMuon> danhSachPhieuMuon;
		ifstream fileIn(fileName);
		string line;

		if (!fileIn.is_open()) {
			cout << RED << "Không thể mở file " << fileName << " để đọc." << endl;
			return danhSachPhieuMuon;
		}

		// Đọc từng dòng trong file và thêm vào vector
		while (getline(fileIn, line)) {
			stringstream ss(line);
			string soPhieuMuonStr, maBanDoc, maSachStr, ngayMuon, ngayPhaiTra, tinhTrangStr;

			getline(ss, soPhieuMuonStr, ',');
			getline(ss, maBanDoc, ',');
			getline(ss, maSachStr, ',');
			getline(ss, ngayMuon, ',');
			getline(ss, ngayPhaiTra, ',');
			getline(ss, tinhTrangStr, ',');

			PhieuMuon phieu(stoi(soPhieuMuonStr), maBanDoc, maSachStr, ngayMuon, ngayPhaiTra, stoi(tinhTrangStr));
			danhSachPhieuMuon.push_back(phieu);
		}

		fileIn.close();
		return danhSachPhieuMuon;
	}

	void ghiDanhSachPhieuMuon() {
		ofstream fileOut("Phieumuon.txt", ios::out | ios::trunc); // Mở file với chế độ ghi đè (truncate)
		if (!fileOut.is_open()) {
			cout << RED << "Không thể mở file " << "Phieumuon.txt" << " để ghi." << endl;
			return;
		}

		// Ghi lại danh sách phiếu mượn vào file
		for (auto& phieu : danhSachPhieuMuon) {
			fileOut << phieu.getSoPhieuMuon() << ","
				<< phieu.getMaBanDoc() << ","
				<< phieu.getMaSach() << ","
				<< phieu.getNgayMuon() << ","
				<< phieu.getNgayPhaiTra() << ","
				<< phieu.getTinhTrang() << endl;
		}

		fileOut.close(); // Đóng file sau khi ghi
	}



	void muonSach(string maSach, string maBanDoc, string ngayMuon, string ngayPhaiTra, vector<PhieuMuon>& dSPhieuMuon, int sophieumuon) {

		bool kiemtramabandoc = false;
		// Kiểm tra mã bạn đọc
		ifstream file("banDoc.txt");

		// Kiểm tra nếu không mở được file
		if (file.is_open()) {
			string line;
			while (getline(file, line)) {
				stringstream ss(line);
				string maBD, hoTen, ngayDK;

				// Tách chuỗi theo dấu phẩy
				getline(ss, maBD, ',');
				getline(ss, hoTen, ',');
				getline(ss, ngayDK, ',');

				// So sánh mã bạn đọc với mã trong file
				if (maBD == maBanDoc) {
					kiemtramabandoc = true;
					file.close(); // Đóng file sau khi tìm thấy
				}
			}
		}
		else {
			cout << RED << "Không thể mở file banDoc.txt" << endl;
			return;
		}

		if (!kiemtramabandoc) {
			cout << RED << "Khong tim thay ma ban doc" << endl;
			return;
		}

		// Tạo phiếu mượn mới
		int soPhieuMuon = dSPhieuMuon.size(); // Tăng số phiếu mượn

		PhieuMuon phieu(soPhieuMuon, maBanDoc, maSach, ngayMuon, ngayPhaiTra, 1);
		//danhSachPhieuMuon.push_back(phieu);


		// Cập nhật lại file Sach.txt và PhieuMuon.txt
		capNhatFilePhieuMuon(phieu);
		cout << PINK << "Muon sach thanh cong!" << endl;
	}

	void traSach(int soPhieuMuon) {

		for (auto& phieu : danhSachPhieuMuon) {
			if (phieu.getSoPhieuMuon() == soPhieuMuon && phieu.getTinhTrang() != 0) {
				phieu.setTinhTrang(0); // Cập nhật tình trạng phiếu mượn thành đã trả
				cout << PINK << "Tra sach thanh cong!" << endl;
				LinkedList<Sach> danhSachSach;
				loadDanhSachSach(danhSachSach);
				// Cập nhật lại file Sach.txt và PhieuMuon.txt
				//capNhatFileSach(phieu.getMaSach(), 0); // Cập nhật sách về trạng thái chưa mượn
				 // Cập nhật phiếu mượn
				danhSachSach.capNhatTinhTrangSach(phieu.getMaSach());
				ghiDanhSachPhieuMuon();

				return;
			}

		}

		cout << RED << "Khong tim thay phieu muon hoac sach da duoc tra!" << endl;
	}


	void capNhatFilePhieuMuon(PhieuMuon& phieu) {
		LinkedList<Sach> danhSachSach;
		loadDanhSachSach(danhSachSach);
		ofstream file("Phieumuon.txt", ios::app); // Mở file ở chế độ ghi thêm (append)

		danhSachSach.capNhatTinhTrangSach(phieu.getMaSach());
		// Kiểm tra xem file có mở thành công hay không
		if (file.is_open()) {
			// Ghi thông tin phiếu mượn vào file với các trường cách nhau bằng dấu phẩy
			file << phieu.getSoPhieuMuon() << ","
				<< phieu.getMaBanDoc() << ","
				<< phieu.getMaSach() << ","
				<< phieu.getNgayMuon() << ","
				<< phieu.getNgayPhaiTra() << ","
				<< phieu.getTinhTrang() << endl;
		}
		else {
			cout << RED << "Không thể mở file Phieumuon.txt để ghi." << endl;
			return;
		}


		file.close(); // Đóng file sau khi ghi

	}
};

vector<PhieuMuon> docDanhSachPhieuMuonTuFile() {
	vector<PhieuMuon> danhSachPhieuMuon;
	ifstream file("PhieuMuon.txt");
	if (file.is_open()) {
		string line;
		while (getline(file, line)) {
			stringstream ss(line);
			string soPhieuMuonStr, maBanDoc, maSach, ngayMuon, ngayPhaiTra, tinhTrangStr, tinhtrangstr;
			getline(ss, soPhieuMuonStr, ',');
			getline(ss, maBanDoc, ',');
			getline(ss, maSach, ',');
			getline(ss, ngayMuon, ',');
			getline(ss, ngayPhaiTra, ',');
			getline(ss, tinhtrangstr, ',');

			int soPhieuMuon = stoi(soPhieuMuonStr);
			int tinhtrang = stoi(tinhtrangstr);

			// Tạo đối tượng PhieuMuon và thêm vào danh sách
			PhieuMuon phieu(soPhieuMuon, maBanDoc, maSach, ngayMuon, ngayPhaiTra, tinhtrang);
			danhSachPhieuMuon.push_back(phieu);
		}
	}
	else {
		cout << RED << "Không thể mở file PhieuMuon.txt" << endl;
		return danhSachPhieuMuon;
	}


	file.close();
	return danhSachPhieuMuon;
}

// Hàm để hiển thị menu
void menuQuanLyPhieuMuon(QuanLyPhieuMuon quanLyPhieuMuon) {


	while (true) {
		int luaChon;
		string maBanDoc, maSach, ngayMuon, ngayTra;
		cout << YELLOW << "\n\n";
		cout << PINK << "==== QUAN LY PHIEU MUA ====" << YELLOW << endl;
		cout << "1. HIEN THI THONG TIN PHIEU MUON" << endl;
		cout << "2. MUON SACH" << endl;
		cout << "3. TRA SACH" << endl;
		cout << "0. QUAY LAI MENU CHINH" << endl;
		cout << "CHON CHUC NANG (0-3): ";
		cin >> luaChon;

		switch (luaChon) {
		case 2: // Thêm phiếu mượn
		{
			vector<PhieuMuon> danhSachPhieuMuon = docDanhSachPhieuMuonTuFile();
			quanLyPhieuMuon.setDanhSachPhieuMuon(danhSachPhieuMuon);
			cin.ignore();

			// Nhập mã bạn đọc và kiểm tra định dạng
			do {
				cout << GREENS << "Nhap ma ban doc (3-15 ky tu, chi bao gom chu va so, khong duoc de trong): ";
				getline(cin, maBanDoc);
				if (maBanDoc == "000") {
					system("cls");
					return;
				}
				if (!isValidMa(maBanDoc)) {
					cout << RED << "Ma ban doc khong hop le. Vui long nhap lai hoac nhap '000' de thoat!" << endl;
				}
			} while (!isValidMa(maBanDoc));

			// Nhập mã sách và kiểm tra định dạng
			do {
				cout << GREENS << "Nhap ma sach (3-10 ky tu, chi bao gom chu va so, khong duoc de trong): ";
				getline(cin, maSach);
				if (maSach == "000") {
					system("cls");
					return;
				}
				if (!isValidMa(maSach)) {
					cout << RED << "Ma sach khong hop le. Vui long nhap lai nhap '000' de thoat!" << endl;
				}
			} while (!isValidMa(maSach));

			// Nhập ngày mượn và kiểm tra định dạng
			do {
				cout << GREENS << "Nhap ngay muon (dd/mm/yyyy, khong duoc de trong): ";
				getline(cin, ngayMuon);
				if (ngayMuon == "000") {
					system("cls");
					return;
				}
				if (!isValidDate(ngayMuon)) {
					cout << RED << "Ngay muon khong hop le. Vui long nhap lai nhap '000' de thoat!" << endl;
				}
			} while (!isValidDate(ngayMuon));


			int sophieumuon = 0;
			if (quanLyPhieuMuon.getDanhSachPhieuMuon().size() > 0) {
				for (int i = 0;i < quanLyPhieuMuon.getDanhSachPhieuMuon().size();i++) {
					if (i == quanLyPhieuMuon.getDanhSachPhieuMuon().size() - 1) {
						sophieumuon = quanLyPhieuMuon.getDanhSachPhieuMuon()[i].getSoPhieuMuon();
					}
				}
			}

			int sachdaduocmuon = false;
			for (auto& val : quanLyPhieuMuon.getDanhSachPhieuMuon()) {
				if (val.getMaSach() == maSach && val.getTinhTrang() == 1) {
					sachdaduocmuon = true;
					cout << RED << "Sach da co nguoi muon \n";
					break;
				}
			}
			if (sachdaduocmuon) {
				break;
			}
			ngayTra = tinhNgayPhaiTra(ngayMuon, 7);

			PhieuMuon phieu(sophieumuon, maBanDoc, maSach, ngayMuon, ngayTra, 1);
			vector<PhieuMuon> dSPhieuMuon = quanLyPhieuMuon.getDanhSachPhieuMuon();
			dSPhieuMuon.push_back(phieu);
			quanLyPhieuMuon.setDanhSachPhieuMuon(dSPhieuMuon);
			quanLyPhieuMuon.muonSach(maSach, maBanDoc, ngayMuon, ngayTra, dSPhieuMuon, sophieumuon);
			cout << PINK << "Them phieu muon thanh cong!" << endl;
			break;
		}

		case 1: // Hiển thị danh sách phiếu mượn
		{
			vector<PhieuMuon> danhSachPhieuMuon = docDanhSachPhieuMuonTuFile();
			quanLyPhieuMuon.setDanhSachPhieuMuon(danhSachPhieuMuon);
			cout << PINK << "Danh sach phieu muon:" << endl;

			quanLyPhieuMuon.hienThiPhieuMuon();

			break;
		}

		case 3: // Trả phiếu
		{
			vector<PhieuMuon> danhSachPhieuMuon = docDanhSachPhieuMuonTuFile();
			quanLyPhieuMuon.setDanhSachPhieuMuon(danhSachPhieuMuon);

			cout << GREENS << "Nhap ma sach de cap nhat tinh trang: ";
			cin >> maSach;

			// Cập nhật tình trạng phiếu mượn (giả sử bạn đã có phương thức tương ứng trong class PhieuMuon)

			quanLyPhieuMuon.traSach(stoi(maSach));

			break;
		}

		case 0: // Thoát
			cout << PINK << "Thoat khoi chuong trinh." << endl;
			system("cls");
			return;
		default:
			cout << RED << "Lua chon khong hop le. Vui long chon lai." << endl;
			break;
		}
		cout << endl; // Xuống dòng
	}
}

int mainQuanLyPhieuMua() {
	vector<PhieuMuon> danhSachPhieuMuon = docDanhSachPhieuMuonTuFile();
	QuanLyPhieuMuon quanLyPhieuMuon;
	quanLyPhieuMuon.setDanhSachPhieuMuon(danhSachPhieuMuon);
	menuQuanLyPhieuMuon(quanLyPhieuMuon);
	return 0;
}
