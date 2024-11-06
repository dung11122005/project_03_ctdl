#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <regex>

using namespace std;

// bảng màu
#define RESET "\033[0m"
#define YELLOW "\033[1;33m"
#define GREEN "\033[0;32m"
#define GREENS "\033[38;5;82m"
#define BLUE "\033[34m"
#define PINK "\033[38;5;213m"
#define RED "\033[0;31m"

class Sach {
private:
	string maSach;
	string tenSach;
	string tacGia;
	string nhaXuatBan;
	double giaBan;
	int namPhatHanh;
	int soTrang;
	string ngayNhapKho;
	int tinhTrang;

public:
	// Constructor mặc định
	Sach() {
		maSach = "";
		tenSach = "";
		tacGia = "";
		nhaXuatBan = "";
		giaBan = 0.0;
		namPhatHanh = 0;
		soTrang = 0;
		ngayNhapKho = "";
		tinhTrang = 0;
	}


	Sach(string ma, string ten, string tacGia, string nxb, double gia, int nam, int trang, string ngayNhap, int tinhTrang) {
		this->maSach = ma;
		this->tenSach = ten;
		this->tacGia = tacGia;
		this->nhaXuatBan = nxb;
		this->giaBan = gia;
		this->namPhatHanh = nam;
		this->soTrang = trang;
		this->ngayNhapKho = ngayNhap;
		this->tinhTrang = tinhTrang; // Sách mới thì tinh trạng luôn là 0
	}


	// Các getter và setter
	string getMaSach() {
		return maSach;
	}

	void setMaSach(string maSach) {
		this->maSach = maSach;
	}

	string getTenSach() {
		return tenSach;
	}

	void setTenSach(string tenSach) {
		this->tenSach = tenSach;
	}

	string getTacGia() {
		return tacGia;
	}

	void setTacGia(string tacGia) {
		this->tacGia = tacGia;
	}

	string getNhaXuatBan() {
		return nhaXuatBan;
	}

	void setNhaXuatBan(string nhaXuatBan) {
		this->nhaXuatBan = nhaXuatBan;
	}

	double getGiaBan() {
		return giaBan;
	}

	void setGiaBan(double giaBan) {
		this->giaBan = giaBan;
	}

	int getNamPhatHanh() {
		return namPhatHanh;
	}

	void setNamPhatHanh(int namPhatHanh) {
		this->namPhatHanh = namPhatHanh;
	}

	int getSoTrang() {
		return soTrang;
	}

	void setSoTrang(int soTrang) {
		this->soTrang = soTrang;
	}

	string getNgayNhapKho() {
		return ngayNhapKho;
	}

	void setNgayNhapKho(string ngayNhapKho) {
		this->ngayNhapKho = ngayNhapKho;
	}

	int getTinhTrang() {
		return tinhTrang;
	}

	void setTinhTrang(int tinhTrang) {
		this->tinhTrang = tinhTrang;
	}
};

// Hàm kiểm tra định dạng mã (chỉ chứa chữ cái và chữ số, độ dài từ 3 đến 10)
bool isValidString(const string& ma) {
	// Kiểm tra chuỗi không rỗng hoặc chỉ chứa dấu cách
	return regex_match(ma, regex("^[A-Za-z0-9]{3,15}$"));
}

// Hàm kiểm tra định dạng ngày theo dạng dd/mm/yyyy
bool checkdDate(const string& date) {
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


template <typename T>
class Node {
public:
	T data;
	Node* next;

	Node(T data) {
		this->data = data;
		this->next = nullptr;
	}
};


template <typename T>
class LinkedList {
private:
	Node<T>* head;

public:
	LinkedList() {
		head = nullptr;
	}

	//hàm tìm sách theo mã
	Node<T>* timTheoMa(const string ma) {
		Node<T>* current = head;
		while (current != nullptr) {
			if (current->data.getMaSach() == ma) {
				return current; // Trả về node tìm thấy
			}
			current = current->next;
		}
		return nullptr; // Không tìm thấy
	}



	// Thêm phần tử vào cuối danh sách và ghi vào file
	bool themCuoi(Sach data) {

		Node<T>* newNode = timTheoMa(data.getMaSach());

		if (newNode != nullptr) {
			cout << RED << "Ma sach da ton tai vui long nhap ma sach khac!\n";
			return false;
		}



		newNode = new Node<T>(data);
		if (head == nullptr) {
			head = newNode;
		}
		else {
			Node<T>* temp = head;
			while (temp->next != nullptr) {
				temp = temp->next;
			}
			temp->next = newNode;
		}

		// Ghi sách mới vào cuối file Sach.txt
		ofstream file("Sach.txt", ios::app); // Mở file ở chế độ ghi nối thêm
		if (file.is_open()) {
			file << data.getMaSach() << ","
				<< data.getTenSach() << ","
				<< data.getTacGia() << ","
				<< data.getNhaXuatBan() << ","
				<< data.getGiaBan() << ","
				<< data.getNamPhatHanh() << ","
				<< data.getSoTrang() << ","
				<< data.getNgayNhapKho() << ","
				<< data.getTinhTrang() << endl;
			file.close();
		}
		else {
			cout << RED << "Không thể mở file Sach.txt để ghi." << endl;
			return false;
		}
		return true;
	}

	//hàm thêm danh sách vào list
	void themVaoList(T data) {
		Node<T>* newNode = new Node<T>(data);
		if (head == nullptr) {
			head = newNode;
		}
		else {
			Node<T>* temp = head;
			while (temp->next != nullptr) {
				temp = temp->next;
			}
			temp->next = newNode;
		}
	}

	// Hiển thị tất cả phần tử trong danh sách và đọc từ file Sach.txt
	void hienThi() {
		ifstream file("Sach.txt");

		if (file.is_open()) {
			string line;

			// Hiển thị tiêu đề các cột
			cout << setw(10) << GREENS << "Ma Sach"
				<< setw(15) << "Ten Sach"
				<< setw(25) << "Tac Gia"
				<< setw(15) << "NXB"
				<< setw(20) << "Gia Ban"
				<< setw(10) << "Nam PH"
				<< setw(10) << "So Trang"
				<< setw(15) << "Ngay Nhap Kho"
				<< setw(13) << "Tinh Trang" << endl;

			cout << "-----------------------------------------------------------------------------------------------------------------------------------" << YELLOW << endl;

			while (getline(file, line)) {
				stringstream ss(line);
				string maSach, tenSach, tacGia, nhaXuatBan, giaBan, namPhatHanh, soTrang, ngayNhapKho, tinhTrang;

				// Tách chuỗi theo dấu phẩy
				getline(ss, maSach, ',');
				getline(ss, tenSach, ',');
				getline(ss, tacGia, ',');
				getline(ss, nhaXuatBan, ',');
				getline(ss, giaBan, ',');
				getline(ss, namPhatHanh, ',');
				getline(ss, soTrang, ',');
				getline(ss, ngayNhapKho, ',');
				getline(ss, tinhTrang, ',');

				// In ra các giá trị theo cột
				cout << left << setw(10) << maSach
					<< setw(25) << tenSach
					<< setw(25) << tacGia
					<< setw(15) << nhaXuatBan
					<< setw(10) << giaBan
					<< setw(10) << namPhatHanh
					<< setw(10) << soTrang
					<< setw(15) << ngayNhapKho
					<< setw(10) << (tinhTrang == "0" ? BLUE"chua muon" : RED"da muon") << YELLOW << endl;
			}

			file.close();
		}
		else {
			cout << RED << "Không thể mở file Sach.txt để đọc." << endl;
		}
	}


	// Xóa phần tử theo mã và cập nhật lại file Sach.txt
	bool xoaTheoMa(const string& ma) {
		if (head == nullptr) return false;

		// Nếu phần tử đầu tiên có mã cần xóa
		if (head->data.getMaSach() == ma) {
			// Kiểm tra trạng thái sách, nếu đang cho mượn (tinhTrang = 1) thì không cho xóa
			if (head->data.getTinhTrang() == 1) {
				cout << RED << "Sach dang duoc muon, khong the xoa!" << endl;
				return false;
			}

			// Xóa phần tử đầu tiên
			Node<T>* temp = head;
			head = head->next;
			delete temp;

			// Ghi đè lại danh sách mới vào file
			capNhatFile();
			return true;
		}

		// Tìm và xóa phần tử có mã cần xóa
		Node<T>* current = head;
		Node<T>* previous = nullptr;
		head;
		while (current != nullptr) {
			if (current->data.getMaSach() == ma) {
				// Kiểm tra trạng thái sách, nếu đang cho mượn (tinhTrang = 1) thì không cho xóa
				if (current->data.getTinhTrang() == 1) {
					cout << RED << "Sach dang duoc muon, khong the xoa!" << endl;
					return false;
				}

				// Xóa phần tử
				previous->next = current->next;
				delete current;

				// Ghi đè lại danh sách mới vào file
				capNhatFile();
				return true;
			}

			previous = current;
			current = current->next;
		}
		cout << RED << "Khong tim thay ma sach can xoa!";
		return false; // Không tìm thấy sách cần xóa
	}




	// Hàm hỗ trợ ghi lại toàn bộ danh sách vào file (sử dụng sau khi xóa hoặc cập nhật)
	bool capNhatFile() {
		ofstream file("Sach.txt", ios::out); // Mở file với chế độ ghi đè hoàn toàn
		if (file.is_open()) {
			Node<T>* temp = head;
			while (temp != nullptr) {
				file << temp->data.getMaSach() << ","
					<< temp->data.getTenSach() << ","
					<< temp->data.getTacGia() << ","
					<< temp->data.getNhaXuatBan() << ","
					<< temp->data.getGiaBan() << ","
					<< temp->data.getNamPhatHanh() << ","
					<< temp->data.getSoTrang() << ","
					<< temp->data.getNgayNhapKho() << ","
					<< temp->data.getTinhTrang() << endl;
				temp = temp->next;
			}
			file.close();
		}
		else {
			cout << RED << "Không thể mở file Sach.txt để ghi." << endl;
			return false;
		}
		return true;
	}

	// Hàm cập nhật tinhTrang của sách và ghi lại vào file
	bool capNhatTinhTrangSach(const string& maSach) {
		// Tìm node sách theo mã
		Node<T>* nodeSach = timTheoMa(maSach);

		if (nodeSach == nullptr) {
			cout << RED << "Không tìm thấy sách với mã: " << maSach << endl;
			return false;
		}


		// Cập nhật tình trạng mới
		if (nodeSach->data.getTinhTrang() == 0) {
			nodeSach->data.setTinhTrang(1);
		}
		else if (nodeSach->data.getTinhTrang() == 1) {
			nodeSach->data.setTinhTrang(0);
		}

		// Ghi lại toàn bộ danh sách sách vào file để cập nhật tình trạng

		if (!capNhatFile()) {
			return false;
		}

		cout << PINK << "Cap nhat tinh trang sach thanh cong!" << endl;
		return true;
	}
};

//hàm load danh sách sách ra từ file và đưa vào LinkedList<Sach>& danhSachSach
void loadDanhSachSach(LinkedList<Sach>& danhSachSach) {
	ifstream file("Sach.txt");
	if (file.is_open()) {
		string line;
		while (getline(file, line)) {
			stringstream ss(line);
			string maSach, tenSach, tacGia, nhaXuatBan, giaBan, namPhatHanh, soTrang, ngayNhapKho, tinhTrangstr;

			// Tách chuỗi theo dấu phẩy
			getline(ss, maSach, ',');
			getline(ss, tenSach, ',');
			getline(ss, tacGia, ',');
			getline(ss, nhaXuatBan, ',');
			getline(ss, giaBan, ',');
			getline(ss, namPhatHanh, ',');
			getline(ss, soTrang, ',');
			getline(ss, ngayNhapKho, ',');
			getline(ss, tinhTrangstr, ',');

			// Chuyển đổi các dữ liệu số
			double gia = stod(giaBan);
			int nam = stoi(namPhatHanh);
			int trang = stoi(soTrang);
			int tinhTrang = stoi(tinhTrangstr);

			// Tạo đối tượng Sach mới
			Sach sach(maSach, tenSach, tacGia, nhaXuatBan, gia, nam, trang, ngayNhapKho, tinhTrang);

			// Thêm đối tượng vào danh sách
			danhSachSach.themVaoList(sach);
		}
		file.close();
	}
	else {
		cout << RED << "Khong the mo file Sach.txt de doc du lieu!" << endl;
	}
}

//hàm hiển thị menu quản lí sách
void menuQuanLySach(LinkedList<Sach>& danhSachSach) {

	while (true) {
		cout << YELLOW << "\n\n";
		cout << PINK << "==== QUAN LY SACH ====" << YELLOW << endl;
		cout << "1. HIEN THI THONG TIN SACH" << endl;
		cout << "2. THEM SACH" << endl;
		cout << "3. XOA SACH" << endl;
		cout << "0. QUAY LAI MENU CHINH" << endl;
		cout << "CHON CHUC NANG (0-3): ";
		int chon;
		cin >> chon;
		switch (chon) {

			// hiển thị thông tin sách
		case 1:
			loadDanhSachSach(danhSachSach);
			danhSachSach.hienThi();
			break;


			// thêm sách
		case 2: {
			loadDanhSachSach(danhSachSach);
			string ma, ten, tacGia, nxb, ngayNhap;
			double gia;
			int nam, trang;
			cin.ignore();

			do {
				cout << GREENS << "Nhap ma sach (khong duoc rong): ";
				getline(cin, ma);
				if (ma == "000") {
					system("cls");
					return;
				}
				if (!isValidString(ma)) {
					cout << RED << "Ma sach khong hop le. Vui long nhap lai hoac nhan '000' de thoat !" << endl;
				}

			} while (!isValidString(ma));

			// Nhập tên sách và kiểm tra
			cout << GREENS << "Nhap ten sach (khong duoc rong): ";
			getline(cin, ten);


			// Nhập tác giả và kiểm tra
			cout << GREENS << "Nhap tac gia (khong duoc rong): ";
			getline(cin, tacGia);


			// Nhập nhà xuất bản và kiểm tra
			cout << GREENS << "Nhap nha xuat ban (khong duoc rong): ";
			getline(cin, nxb);


			// Nhập giá bán và kiểm tra
			do {
				cout << GREENS << "Nhap gia ban (so duong, vi du: 100000): ";
				cin >> gia;
				if (gia == 0) {
					system("cls");
					return;
				}
				if (cin.fail() || gia < 0) {
					cout << RED << "Gia ban khong hop le. Vui long nhap lai hoac nhap so '0' de thoat!" << endl;
				}
			} while (gia <= 0);

			// Nhập năm phát hành và kiểm tra
			do {
				cout << GREENS << "Nhap nam phat hanh (nam duong, vi du: 2024): ";
				cin >> nam;
				if (nam == 0) {
					system("cls");
					return;
				}
				if (cin.fail() || nam < 0) {
					cout << RED << "Nam phat hanh khong hop le. Vui long nhap lai hoac nhap so '0' de thoat!" << endl;
				}
			} while (nam <= 0);

			// Nhập số trang và kiểm tra
			do {
				cout << GREENS << "Nhap so trang (so nguyen duong, vi du: 100): ";
				cin >> trang;
				if (trang == 0) {
					system("cls");
					return;
				}
				if (cin.fail() || trang <= 0) {
					cout << RED << "So trang khong hop le. Vui long nhap lai hoac nhap so '0' de thoat!" << endl;
				}
			} while (trang <= 0);

			// Nhập ngày nhập kho và kiểm tra định dạng
			cin.ignore();  // Xóa ký tự xuống dòng còn lại từ lần nhập trước
			do {
				cout << GREENS << "Nhap ngay nhap kho (dd/mm/yyyy): ";
				getline(cin, ngayNhap);
				if (ngayNhap == "000") {
					system("cls");
					return;
				}
				if (!checkdDate(ngayNhap)) {
					cout << RED << "Ngay nhap kho khong hop le. Vui long nhap lai nhap '000' de thoat!" << endl;
				}
			} while (!checkdDate(ngayNhap));

			Sach sachMoi(ma, ten, tacGia, nxb, gia, nam, trang, ngayNhap, 0);
			if (danhSachSach.themCuoi(sachMoi)) {
				cout << PINK << "Them sach thanh cong \n";
			}

			break;
		}

			  //xóa sách
		case 3: {
			LinkedList<Sach> dss;
			string ma;
			loadDanhSachSach(dss);
			cout << YELLOW << "Nhap ma sach can xoa:  ";
			cin >> ma;
			if (dss.xoaTheoMa(ma)) {
				cout << PINK << "Da xoa sach thanh cong\n";
			}
			break;
		}
		case 0:
			cout << PINK << "QUAY LAI MENU CHINH." << endl;
			system("cls");
			return;
		default:
			cout << RED << "Lua chon khong hop le. Vui long chon lai." << endl;
			break;
		}

	}

}

int mainQuanLySach() {
	//khởi tạo một sanh sách sách
	LinkedList<Sach> danhSachSach;

	//load sách từ file và đừa vào danhSachSach
	loadDanhSachSach(danhSachSach);

	//đưa danh sách sách vào menu để thực hiện chức năng
	menuQuanLySach(danhSachSach);

	return 0;
}

