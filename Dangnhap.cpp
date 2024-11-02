#include <iostream>
#include <fstream>
#include <string.h>
#include <windows.h>
#include <iomanip>
#include <conio.h>
#include <string>
#include <cstdlib>
#include <chrono>
#include <thread>
#include "quanLiSach.h"
#include "quanLiPhieuMuon.h"

using namespace std;


#define RESET "\033[0m"
#define YELLOW "\033[1;33m"
#define GREEN "\033[0;32m"
#define GREENS "\033[38;5;82m"
#define BLUE "\033[34m"
#define PINK "\033[38;5;213m"
#define RED "\033[0;31m"


const int MAX_ATTEMPTS = 3;


string getHiddenInput() {
	string input;
	char ch;

	while (true) {
		ch = _getch();  // Đọc từng ký tự mà không hiển thị nó

		if (ch == '\r') {  // Kiểm tra phím Enter (mã ASCII 13)
			break;
		}
		else if (ch == '\b') {  // Kiểm tra phím Backspace (mã ASCII 8)
			if (!input.empty()) {
				cout << "\b \b";  // Xóa ký tự cuối cùng
				input.pop_back();
			}
		}
		else {
			input += ch;  // Thêm ký tự vào chuỗi
			cout << '*';  // Hiển thị ký tự '*'
		}
	}

	cout << endl;  // Xuống dòng sau khi hoàn thành việc nhập
	return input;
}

bool hashPasswordlogin(string password, string filePassword) {
	hash <string> hash;

	unsigned long hashedPasswordGuess = hash(password);

	string passwordhash = to_string(hashedPasswordGuess);

	//cout << hashedPasswordGuess << endl;

	if (filePassword == passwordhash) {
		return true;
	}
	else {
		return false;
	}
}


bool loginAuthentication(string username, string password) {
	ifstream file("Admin.txt");
	if (!file) {
		cerr << "Không thể mở file Admin.txt" << endl;
		return false;
	}
	string fileUsername, filePassword;
	while (file >> fileUsername >> filePassword) {
		if (username == fileUsername && hashPasswordlogin(password, filePassword)) {
			return true;
		}
	}
	return false;
}


bool login() {
	string user, password;
	cout << "\n\n\n\n";
	cout << setw(65) << YELLOW << "********************************\n";
	cout << setw(65) << YELLOW << "*" << GREENS << "       DANG NHAP HE THONG    " << YELLOW << "*\n";
	cout << setw(65) << YELLOW << "********************************\n";
	cout << GREEN << setw(70) << "User: ";
	cout << RESET;
	cin >> user;cout << "\n\n";
	cout << GREEN << setw(74) << "Password: ";
	cout << RESET;
	password = getHiddenInput();

	if (loginAuthentication(user, password)) {
		return true;
	}
	else {
		return false;
	}
}


bool menu() {
	while (true) {
		cout << "\n\n";
		cout << setw(20) << YELLOW << "==== MENU ====\n\n";
		cout << setw(20) << PINK << "1. QUAN LY SACH\n\n";
		cout << setw(20) << PINK << "2. QUAN LY PHIEU MUON\n\n";
		cout << setw(20) << PINK << "0. THOAT\n";
		cout << RESET;
		cout << "\n\n\n";

		int choice;
		cout << GREENS << "LUA CHON CUA BAN LA: ";
		cin >> choice;
		switch (choice) {
		case 1:
			cout << YELLOW << "1. QUAN LY SACH.\n\n";
			mainQuanLySach();
			break;
		case 2:
			cout << YELLOW << "2. QUAN LY PHIEU SACH.\n\n";
			mainQuanLyPhieuMua();
			break;
		case 0:
			cout << YELLOW << "0. THOAT CHUONG TRINH.\n\n";
			return false;
		default:
			cout << RED << "LUA CHON KHONG HOP LE, VUI LONG CHON LAI.\n";
		}
	}
}


int main()
{
	for (int i = 0;i < 3;i++) {
		if (login()) {
			system("cls");
			if (!menu()) {
				return 0;
			}
		}
		else if (i == 2) {
			cout << "\n\n";
			cout << setw(50) << RED << "DANG NHAP SAI QUA 3 LAN, CHUONG TRINH SE DONG SAU 2 GIAY\n\n\n\n";
			cout << RESET;
			this_thread::sleep_for(std::chrono::seconds(2));
			return 0;
		}
		else {
			system("cls");
		}
	}

	cout << RESET;
	return 0;
}