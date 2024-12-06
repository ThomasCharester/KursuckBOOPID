#pragma once
#ifndef USERINTERFACE
#define USERINTERFACE
#include "Database.h"
#include <iomanip>
#include <iostream>
#include <conio.h>
#include "Windows.h"


namespace realtyPriceRate {
	//#define SOUND

#define DEFAULTCOLOR 7

#define ANIMATIONSPEED 30

	class UI {
	private:
		/// <summary>
		/// ��������� �� ���� ������
		/// </summary>
		Database* db = nullptr;
		/// <summary>
		/// ����������� ������ �� ��������.
		/// </summary>
		static UI* instance;

	public:
		/// <summary>
		/// �������� ������������ ��������� UI.
		/// </summary>
		/// <returns>������������ ��������� UI.</returns>
		static UI* GetInstance() {
			return instance;
		}
		// ����� 0-�����,1-�������,2-�����,3-������,4-���������,5-�������,6-Ƹ����.
		enum colors : int {
			Default,
			White,
			Red,
			Blue,
			Green,
			Orange,
			Pink,
			Yellow
		};
		/// <summary>
		/// ����������� ������ UI.
		/// </summary>
		/// <param name="defaultColor">���� �� ���������.</param>
		UI(UI::colors defaultColor = White) {
			if (instance != nullptr) delete this;

			instance = this;

			setlocale(LC_ALL, "Russian");
			this->defaultColor = defaultColor;
			setColor(defaultColor);
		}
		/// <summary>
		/// ������ ��������� �� ���� ������ ����� ���������.
		/// </summary>
		/// <param name="db">�� ����� ���� ������.</param>
		void setDatabase(Database* db) { this->db = db; }
		/// <summary>
		/// ������ ��������� �� ���� ������ �� ������.
		/// </summary>
		/// <param name="db">�� ����� ���� ������.</param>
		void setDatabase(Database& db) { this->db = &db; }
		/// <summary>
		/// ���� ������ ������� �� ���������.
		/// </summary>
		UI::colors defaultColor = White;

		/// <summary>
		/// ����� ����� ������ �������.
		/// </summary>
		/// <param name="color">��������������� ����.</param>
		void setColor(UI::colors color) {
			HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
			switch (color) {
			case UI::colors::White:
				SetConsoleTextAttribute(handle, 7);
				break;
			case UI::colors::Red:
				SetConsoleTextAttribute(handle, 12);
				break;
			case UI::colors::Green:
				SetConsoleTextAttribute(handle, 10);
				break;
			case UI::colors::Blue:
				SetConsoleTextAttribute(handle, 9);
				break;
			case UI::colors::Yellow:
				SetConsoleTextAttribute(handle, 14);
				break;
			case UI::colors::Orange:
				SetConsoleTextAttribute(handle, 6);
				break;
			case UI::colors::Pink:
				SetConsoleTextAttribute(handle, 13);
				break;
			default:
				setColor(defaultColor);
				break;
			}
		}
		/// <summary>
		/// ���� ������ ��������� ����.
		/// </summary>
		/// <typeparam name="T">��� �������� ������.</typeparam>
		/// <param name="text">��������� ������������.</param>
		/// <param name="newLine">���������� �� �� ��������� ������.</param>
		/// <returns>�������� ������.</returns>
		template<typename T>
		T input(string text, bool newLine = true) {
			T input{};
			while (true) {
				printColor(text + " : ", newLine, true);
#ifdef SOUND
				cout << (char)7;
#endif
				SetConsoleCP(1251);
				cin >> input;
				SetConsoleCP(866);
				if (cin.fail()) {
					MusicPlayer::playSound(sounds::Button_Failure);
					printColor("&2������������ ��� ������!");
					cin.clear();
					cin.ignore(100500, '\n');
				}
				else {
					return input;
				}
			}
		}
		/// <summary>
		/// ���� ������ ��� ������ ��������. ������������ ��� ����� ������.
		/// </summary>
		/// <param name="text">��������� ������������.</param>
		/// <param name="newLine">���������� �� �� ��������� ������.</param>
		/// <returns>�������� ������.</returns>
		string _input(string text, bool newLine = true) {
			string input = "";
			printColor(text + " : ", newLine, true);
#ifdef SOUND 
			cout << (char)7;
#endif
			SetConsoleCP(1251);
			char ch = _getch();
			do {
				if ((int)ch == 8 && !input.empty()) {
					cout << '\b' << ' ' << '\b';
					input.erase(input.begin() + input.size() - 1);
				}
				else {
					cout << '*';
					input += ch;
				}
				ch = _getch();
				MusicPlayer::playSound(sounds::Button_Press2);
			} while (ch != 13);
			SetConsoleCP(866);
			return input;
		}
		/// <summary>
		/// ������������ ���� int �� 0 �� 9, �� ������� ������� Enter. ������������ � ��������.
		/// </summary>
		/// <param name="text">��������� ������������.</param>
		/// <param name="min">����� �������.</param>
		/// <param name="max">������ �������.</param>
		/// <param name="newLine">���������� �� �� ��������� ������.</param>
		/// <returns>����� �������� �������������.</returns>
		int inputRangeInstant(std::string text, int min = 0, int max = 9, bool newLine = true) {
			int input;
			while (true) {
				printColor(text + " : ", newLine, true);
#ifdef SOUND 
				cout << (char)7;
#endif
				input = _getch();
				if (input > 48 + max || input < min + 48) {
					printColor("&2������� �������� � ��������� �� " + to_string(min) + " �� " + to_string(max));
				}
				else {
					MusicPlayer::playSound(Button_Press1);
					return input - 48;
				}
			}
		}
		/// <summary>
		/// ���� ������ � ���������.
		/// </summary>
		/// <typeparam name="T">��� �������� ������.</typeparam>
		/// <param name="text">��������� ������������.</param>
		/// <param name="min">����� �������.</param>
		/// <param name="max">������ �������.</param>
		/// <param name="newLine">���������� �� �� ��������� ������.</param>
		/// <returns>�������� ������ � ���������.</returns>
		template<typename T>
		T inputRange(string text, T min, T max, bool newLine = true) {
			T input{};
			while (true) {
				printColor(text + " : ", newLine, true);
#ifdef SOUND
				cout << (char)7;
#endif
				cin >> input;
				if (cin.fail()) {
					MusicPlayer::playSound(sounds::Button_Failure);
					printColor("&2������������ ��� ������!");
					cin.clear();
					cin.ignore(100500, '\n');
				}
				else if ((max != min) && (input > max || input < min)) {
					MusicPlayer::playSound(sounds::Button_Failure);
					printColor("&2������� �������� � ��������� �� " + to_string(min) + " �� " + to_string(max));
				}
				else {
					return input;
				}
			}
		}
		/// <summary>
		/// ������� ������ � �������.
		/// </summary>
		/// <typeparam name="T">��� ��������� ������.</typeparam>
		/// <param name="data">������ �������� ������������.</param>
		/// <param name="newLine">���������� �� �� ��������� ������.</param>
		template <typename T>
		void print(T data, bool newLine = true) {
			SetConsoleCP(1251);
			if (newLine)  cout << '\n';
			cout << data;
			SetConsoleCP(866);
		}
		/// <summary>
		/// ����� ������ � ������� � �������������� �����. ������ �������� ������ &, ������� ����� �������� �����, ���������� �� ���.
		/// </summary>
		/// <param name="str">������ ��� ������.</param>
		/// <param name="newLine">���������� �� �� ��������� ������.</param>
		/// <param name="animation">�������� ����� ������� ��������.</param>
		void printColor(string str, bool newLine = true, bool animation = false) {
			SetConsoleCP(1251);
			if (newLine)  cout << '\n';
			bool flag = false;
			for (char ch : str) {
				if (animation) Sleep(ANIMATIONSPEED);
#ifdef SOUND
				cout << (char)7;
#endif
				if (!flag)
					if (ch != '&')
						cout << ch;
					else flag = true;
				else {
					setColor(static_cast<UI::colors>((ch - 48)));
					flag = false;
				}
			}
			SetConsoleCP(866);
			setColor(defaultColor);

		}
		/// <summary>
		/// ������� ������ ��������.
		/// </summary>
		/// <param name="data">������ ����� � �������</param>
		/// <param name="precision">�����.</param>
		/// <param name="header">��������� �������.</param>
		void printTable(vector<string> data, int precision = -1, bool header = false) {
			int max = data.at(0).length();
			for (string str : data) {
				if (str.length() > max) max = str.length();
			}
			if (precision >= 0) max = precision;
			cout << '\n' << '|';
			SetConsoleOutputCP(1251);
			for (string str : data) {
				cout << setw(max) << left << str << '|';
			}
			SetConsoleOutputCP(866);
			cout << '\n';
			for (int i = 0; i < data.size() * max + data.size() + 1; i++)
				if (header) cout << '=';
				else cout << '-';
		}
		/// <summary>
		/// ������� ���������.
		/// </summary>
		/// <param name="data">����� ���������.</param>
		/// <param name="precision">������.</param>
		/// <param name="fillingChar">������ ��� ����������.</param>
		void printHeader(string data, int precision, char fillingChar = '=')
		{
			cout << '\n';

			int test = (precision - data.size()) / 2;

			for (int i = 0; i < test; i++)
				cout << fillingChar;

			printColor(data, false);

			for (int i = 0; i < test; i++)
				cout << fillingChar;
		}
		/// <summary>
		/// ������� �� ������ ����� �������
		/// </summary>
		void pressAnyButton() {
			cout << '\n';
			system("pause");
		}
		/// <summary>
		/// ������������� ��������.
		/// </summary>
		/// <returns>�������� ��� ����������.</returns>
		const bool confirm() {
			return inputRangeInstant("&0�� �������? (&20-���&0,&41-��&0)", 0, 1);
		}

		// ������ ����	

		void adminMenu() {
			while (true) {
				if (!db->isLoggedIn()) return;
				system("cls");
				printHeader("&0����� ���������� " + db->getLogin(), 50);
				printColor("1 - ������������� ������ ��������");
				printColor("2 - ������������� ������ ���������");
				printColor("0 - �����");
				int choice = inputRangeInstant("�������� �������� �� ������", 0, 2);
				system("cls");
				switch (choice) {
				case 0:
					db->deauthorize();
					return;
					break;
				case 1:
					objectsEditing();
					break;
				case 2:
					accountsEditing();
					break;
				}
			}
		}
		void spectatorMenu() {
			while (true) {
				if (!db->isLoggedIn()) return;
				system("cls");
				printHeader("&0����� ���������� " + db->getLogin(), 50);
				printColor("1 - ���������� �� ������ ��������");
				printColor("2 - ���������� �� ������ �������� �� ��������");
				printColor("3 - ����� � ������ ��������");
				printColor("4 - ������������� ������ ��������");
				printColor("5 - ������� ������ ��������");
				printColor("0 - �����");
				int choice = inputRangeInstant("�������� �������� �� ������", 0, 5);
				system("cls");
				switch (choice) {
				case 0:
					db->deauthorize();
					return;
					break;
				case 1:
					db->showGenericObjectInfo();
					pressAnyButton();
					break;
				case 2:
					db->showRateObjectInfo();
					pressAnyButton();
					break;
				case 3:
					searchMenu();
					break;
				case 4:
					sortMenu();
					break;
				case 5:
					system("cls");
					db->changePassword();
					break;
				}
			}
		}
		void judgeMenu() {
			while (true) {
				if (!db->isLoggedIn()) return;
				system("cls");
				printHeader("&0����� ���������� " + db->getLogin(), 50);
				printColor("1 - ���������� �� ������ ��������");
				printColor("2 - ������� ������");
				printColor("3 - ������� ������ ��������");
				printColor("0 - �����");
				int choice = inputRangeInstant("�������� �������� �� ������", 0, 3);
				system("cls");
				switch (choice) {
				case 0:
					db->deauthorize();
					return;
					break;
				case 1:
					db->showDetailObjectInfo();
					pressAnyButton();
					break;
				case 2:
					system("cls");
					db->rateObject();
					break;
				case 3:
					system("cls");
					db->changePassword();
					break;
				}
			}
		}
		void objectsEditing() {
			while (true) {
				if (!db->isLoggedIn()) return;
				system("cls");
				printHeader("������������� ������ �������", 50);
				printColor("1 - �������� ������");
				printColor("2 - ������� ������");
				printColor("3 - ���������� ������ ��������");
				printColor("4 - ������������� ������ ��������");
				printColor("5 - ����� ������ � ������");
				printColor("0 - �����");
				int choice = inputRangeInstant("�������� �������� �� ������", 0, 5);
				system("cls");
				switch (choice) {
				case 0:
					return;
					break;
				case 1:
					db->addObject();
					break;
				case 2:
					db->removeObject();
					break;
				case 3:
					system("cls");
					db->showDetailObjectInfo();
					pressAnyButton();
					break;
				case 4:
					sortMenu();
					break;
				case 5:
					searchMenu();
					break;
				}
			}
		}
		void accountsEditing() {
			while (true) {
				if (!db->isLoggedIn()) return;
				system("cls");
				printHeader("������������� ������ ���������", 50);
				printColor("1 - �������� �������");
				printColor("2 - ������� �������");
				printColor("3 - ���������� ������ ���������");
				printColor("4 - ������������� �������� ������� ���������");
				printColor("5 - ������� ������ ��������");
				printColor("0 - �����");
				int choice = inputRangeInstant("�������� �������� �� ������", 0, 5);
				system("cls");
				switch (choice) {
				case 0:
					return;
					break;
				case 1:
					db->addAccount();
					break;
				case 2:
					db->removeAccount();
					break;
				case 3:
					system("cls");
					db->showAccounts();
					pressAnyButton();
					break;
				case 4:
					db->grantAccess();
					break;
				case 5:
					system("cls");
					db->changePassword();
					break;
				}
			}
		}
		void sortMenu() {
			while (true) {
				if (!db->isLoggedIn() || db->isEmpty()) {
					MusicPlayer::playSound(Button_Failure);
					return;
				}
				system("cls");
				printHeader("������� ����������", 50);
				printColor("1 - �� ��������");
				printColor("2 - �� ���������� �� �����");
				printColor("3 - �� ����");
				printColor("4 - �� �������");
				printColor("0 - �����");

				int choice = inputRangeInstant("�������� �������� �� ������", 0, 5);
				system("cls");
				switch (choice) {
				case 0:
					return;
					break;
				case 1:
					db->sortObjectRate();
					break;
				case 2:
					db->sortObjectMetersToMetro();
					break;
				case 3:
					db->sortObjectFinalPrice();
					break;
				case 4:
					db->sortObjectSquareMeters();
					break;
				}
				return;
			}
		}
		void searchMenu() {
			while (true) {
				if (!db->isLoggedIn() || db->isEmpty()) {
					MusicPlayer::playSound(Button_Failure);
					return;
				}
				system("cls");
				printHeader("�������� ������", 50);
				printColor("1 - �� ��������");
				printColor("2 - �� ���������� �� �����");
				printColor("0 - �����");
				int choice = inputRangeInstant("�������� �������� �� ������", 0, 2);
				system("cls");
				switch (choice) {
				case 0:
					return;
					break;
				case 1:
					system("cls");
					db->findObjectName();
					pressAnyButton();
					break;
				case 2:
					system("cls");
					db->findObjectMetersToMetro();
					pressAnyButton();
					break;
				}
			}
		}
	};
	UI* UI::instance = nullptr;

	// ����������� �������

	void Database::calculateObjectsRatings()
	{
		for (Object* p : objects)
			p->calculatePrice();
	}

	Database::Database() {
		readObjectsFromFile();
		calculateObjectsRatings();

		this->ui = UI::GetInstance();
	}
	Database::~Database() {
		writeAccountsToFile();
		calculateObjectsRatings();
		writeObjectsToFile();
	}

	void Database::login() {
		readAccountsFromFile();
		if (accounts.empty())
		{
			MusicPlayer::playSound(Warning);
			ui->printColor("&2��� ��������� ��� �����������");
			ui->printColor("�������� ������� �������������� ��� ������ ������ � ����� ������");
			addAccount(1);
			currentAccount = accounts.at(0);
			accounts.at(0)->access = true;
			ui->printColor("&4����������� �������");
			return;
		}
		// ���������� �����������
		bool isRegistered = ui->inputRangeInstant("�� ��� ���������������� � �������? (&20-���&0,&41-��&0)", 0, 1);

		if (!isRegistered) {
			bool wantRegister = ui->inputRangeInstant("������ ������������������? (&20-���&0,&41-��&0)", 0, 1);
			if (wantRegister) {
				addAccount();
				ui->printColor("&4����������� ���������, �������� ������������� ���������������");
			}
			else return;
		}
		while (true) {
			// ��������� �������������� ��������
			try {
				// �����������
				if (currentAccount)
					throw(AccountException(AccountExceptionType::AlreadyAuthorized, "", currentAccount->login, currentAccount->password));
				else {
					system("cls");
					ui->printColor("������� � �������");
					string login;

					login = ui->input<string>("������� �����");
					if (!checkLogin(login))
						throw(AccountException(AccountExceptionType::WrongLogin, "", login));

					int accountID = findID(login);
					if (!accounts.at(accountID)->access)
						throw(AccountException(AccountExceptionType::NoAccess, "", login));


					int attempts = 3;
					string password;

					do {
						password = ui->_input("������� ������");
						if (checkPassword(accountID, password) || attempts-- < 0) {
							break;
						}
						ui->printColor("&2�������� ������!");
						MusicPlayer::playSound(Button_Restricted);
					} while (true);

					if (attempts < 0)
						throw(AccountException(AccountExceptionType::WrongPassword, "&2�� ������� ����� ������ ������ 3 ���, ���������� �����.", login, password));

					MusicPlayer::playSound(Access, true);
					MusicPlayer::playSound(Granted);

					currentAccount = accounts.at(accountID);
					ui->printColor("&4����������� �������");
				}
				return;
			}
			catch (AccountException ex) {

				ui->printColor(ex.what());
				ui->pressAnyButton();

				if (ex.whatType() == AccountExceptionType::AlreadyAuthorized || ex.whatType() == AccountExceptionType::NoAccess || ex.whatType() == AccountExceptionType::WrongLogin) return;

			}
		}
	}

	void Database::showAccounts() {
		if (accounts.empty())
		{
			MusicPlayer::playSound(Button_Failure);
			ui->printColor("&2������ ����");
			return;
		}
		vector<string> table;

		SetConsoleCP(1251);
		table.emplace_back("ID");
		table.emplace_back("�����");
		table.emplace_back("����");
		table.emplace_back("������");

		ui->printTable(table, 15, true);

		for (int i = 0; i < accounts.size(); i++) {
			table.clear();
			table.emplace_back(to_string(i));
			table.emplace_back(accounts.at(i)->login);

			switch (accounts.at(i)->accountType) {
			case 0:
				table.emplace_back("�����");
				break;
			case 1:
				table.emplace_back("��������������");
				break;
			case 2:
				table.emplace_back("�������");
				break;
			}
			if (accounts.at(i)->access) table.emplace_back("��������");
			else table.emplace_back("��������");

			ui->printTable(table, 15);
		}
		SetConsoleCP(866);
	}
	void Database::addAccount(int type) {
		string login;
		while (true) {
			login = ui->input<string>("������� �����");
			if (!checkLogin(login)) break;
			ui->printColor("&2���� ������������ ��� ����������");
		}
		string password = ui->input<string>("������� ������");

		int accountType = type;
		if (type < 0 || type > 2)
			accountType = ui->input<int>("������� ��� �������� (&20-�������&0,&41-�������������&0,&42-�������&0)");

		if (isLoggedIn()) accounts.emplace_back(new Account(login, password, accountType, true));
		else accounts.emplace_back(new Account(login, password, accountType, false));

		MusicPlayer::playSound(Add);

		writeAccountsToFile();
	}
	void Database::removeAccount() {
		showAccounts();

		if (accounts.empty()) {
			MusicPlayer::playSound(Button_Failure);
			ui->printColor("&2 ������ �������");
			return;
		}
		int id = ui->inputRange<int>("�������� ����� ���������� ��������", 0, accounts.size() - 1);

		if (!ui->confirm()) return;

		if (currentAccount == accounts.at(id)) {
			system("cls");
			MusicPlayer::playSound(Error);
			ui->printColor("&2�� �� ������ ������� ������� � ������� ������������");
			return;
		}

		delete accounts.at(id);
		accounts.erase(accounts.begin() + id);

		writeAccountsToFile();

		ui->printColor("&4������� ������� �����");
	}
	void Database::grantAccess()
	{
		showAccounts();
		int id = ui->inputRange<int>("�������� ����� �������� ��� ��������� ��������� �������", 0, accounts.size() - 1);

		if (!ui->confirm()) return;

		if (currentAccount == accounts.at(id)) {
			system("cls");
			MusicPlayer::playSound(Button_Failure, true);
			MusicPlayer::playSound(Error);
			ui->printColor("&2�� �� ������ �������� ������ � ��������, � ������� ������������");
			return;
		}

		accounts.at(id)->access = !accounts.at(id)->access;
		MusicPlayer::playSound(Access, true);
		if (accounts.at(id)->access) MusicPlayer::playSound(Granted);
		else MusicPlayer::playSound(Denied);

		writeAccountsToFile();

		ui->printColor("&4�������� ������� ������");
	}
	void Database::changePassword()
	{
		if (!isLoggedIn()) {
			MusicPlayer::playSound(Error);
			return;
		}

		if (whoIsNow() == 1) {
			// ����� ������ ������ ������
			showAccounts();
			int id = ui->inputRange<int>("�������� ����� �������� ��� ��������� ������", 0, accounts.size() - 1);

			if (!ui->confirm()) return;

			string password = ui->input<string>("������� ����� ������");
			accounts.at(id)->password = password;

			writeAccountsToFile();

			ui->printColor("&4�������� ������� ������");
		}
		else {
			if (!ui->confirm()) return;

			string password = ui->input<string>("������� ����� ������");
			currentAccount->password = password;

			writeAccountsToFile();
		}
	}

	void Database::showGenericObjectInfo() {
		if (objects.empty())
		{
			MusicPlayer::playSound(Button_Failure);
			ui->printColor("&2������ �������� ����");
			return;
		}

		vector<string> table;

		SetConsoleCP(1251);
		table.emplace_back("ID");
		table.emplace_back("���");
		table.emplace_back("��������");
		table.emplace_back("������� ���2");
		table.emplace_back("����");
		table.emplace_back("������ �� �����");

		ui->printTable(table, 15, true);

		for (int i = 0; i < objects.size(); i++) {
			table.clear();
			table.emplace_back(to_string(i));
			objects.at(i)->getGenericInfo(table);
			ui->printTable(table, 15);
		}
		SetConsoleCP(866);
	}
	void Database::showDetailObjectInfo() {
		if (objects.empty())
		{
			MusicPlayer::playSound(Button_Failure);
			ui->printColor("&2������ �������� ����");
			return;
		}

		vector<string> table;

		SetConsoleCP(1251);
		table.emplace_back("ID");
		table.emplace_back("���");
		table.emplace_back("��������");
		table.emplace_back("������� ���2");
		table.emplace_back("����");
		table.emplace_back("������ �� �����");
		table.emplace_back("������");

		ui->printTable(table, 15, true);

		for (int i = 0; i < objects.size(); i++) {
			table.clear();
			table.emplace_back(to_string(i));
			objects.at(i)->getDetailInfo(table);
			ui->printTable(table, 15);
		}
		SetConsoleCP(866);
	}
	void Database::showRateObjectInfo() {
		if (objects.empty())
		{
			MusicPlayer::playSound(Button_Failure);
			ui->printColor("&2������ �������� ����");
			return;
		}
		vector<string> table;

		SetConsoleCP(1251);
		table.emplace_back("ID");
		table.emplace_back("���");
		table.emplace_back("��������");
		table.emplace_back("����");
		table.emplace_back("������");

		ui->printTable(table, 15, true);

		for (int i = 0; i < objects.size(); i++) {
			table.clear();
			table.emplace_back(to_string(i));
			objects.at(i)->getRateInfo(table);
			ui->printTable(table, 15);

		}
		SetConsoleCP(866);
	}

	void Database::addObject() {
		int objectType = ui->inputRange<int>("������� ��� ������������ (1 - ���, 2 - ����)", 1, 2);
		string objectName = ui->input<string>("������� �������� �������");
		int squareMeters = ui->inputRange<int>("������� �������", 0, 1000);
		int metersToMetro = ui->inputRange<int>("������� ���������� �� �����", 0, 10000);

		switch (objectType)
		{
		case 1:
			objects.emplace_back(new House(objectName, squareMeters, 0, metersToMetro, 27000));
			break;
		case 2:
			objects.emplace_back(new Office(objectName, squareMeters, 0, metersToMetro, 27000));
			break;
		}

		MusicPlayer::playSound(Add);

		writeObjectsToFile();
	}
	void Database::removeObject() {
		showGenericObjectInfo();

		if (objects.empty()) {
			MusicPlayer::playSound(Button_Failure);
			ui->printColor("&2 ������ ������� :/");
			return;
		}
		int id = ui->inputRange<int>("�������� ����� ���������� �������", 0, objects.size() - 1);

		if (!ui->confirm()) return;

		delete objects.at(id);
		objects.erase(objects.begin() + id);

		writeObjectsToFile();

		ui->printColor("&����� ������ � ������� ������");
	}
	void Database::rateObject()
	{
		if (objects.empty())
		{
			MusicPlayer::playSound(Button_Failure);
			ui->printColor("&2������ ���������");
			return;
		}
		showGenericObjectInfo();

		int id = ui->inputRange<int>("�������� ����� ������������ �������", 0, objects.size() - 1);
		int rating = ui->inputRange<int>("������� ������", 0, 100);

		objects.at(id)->addRating(getLogin(), rating);
		objects.at(id)->calculatePrice();

		writeObjectsToFile();
	}

	void Database::findObjectName() {
		if (objects.empty())
		{
			MusicPlayer::playSound(Button_Failure);
			ui->printColor("&2������ �������� ����");
			return;
		}
		string objectName = ui->input<string>("������� �������� ��� ������: ");

		vector<string> table;

		SetConsoleCP(1251);
		table.emplace_back("ID");
		table.emplace_back("���");
		table.emplace_back("��������");
		table.emplace_back("������� ���2");
		table.emplace_back("����");
		table.emplace_back("������ �� �����");
		table.emplace_back("������");

		ui->printTable(table, 15, true);

		for (int i = 0; i < objects.size(); i++) {
			if (*objects.at(i) == objectName)
			{
				table.clear();
				table.emplace_back(to_string(i));
				objects.at(i)->getDetailInfo(table);
				ui->printTable(table, 15);
			}
		}
		SetConsoleCP(866);
	}
	void Database::findObjectMetersToMetro() {
		if (objects.empty())
		{
			MusicPlayer::playSound(Button_Failure);
			ui->printColor("&2������ �������� ����");
			return;
		}
		int metersToMetro = ui->input<int>("������� ���������� ��� ������: ");

		vector<string> table;

		SetConsoleCP(1251);
		table.emplace_back("ID");
		table.emplace_back("���");
		table.emplace_back("��������");
		table.emplace_back("������� ���2");
		table.emplace_back("����");
		table.emplace_back("������ �� �����");
		table.emplace_back("������");

		ui->printTable(table, 15, true);

		for (int i = 0; i < objects.size(); i++) {
			if (*objects.at(i) == metersToMetro)
			{
				table.clear();
				table.emplace_back(to_string(i));
				objects.at(i)->getDetailInfo(table);
				ui->printTable(table, 15);
			}
		}

		SetConsoleCP(866);
	}
}
#endif // !UI
