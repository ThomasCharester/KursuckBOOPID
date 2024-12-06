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
		/// Указатель на базу данных
		/// </summary>
		Database* db = nullptr;
		/// <summary>
		/// Статическая ссылка на одиночку.
		/// </summary>
		static UI* instance;

	public:
		/// <summary>
		/// Получить единственный экземпляр UI.
		/// </summary>
		/// <returns>Единственный экземпляр UI.</returns>
		static UI* GetInstance() {
			return instance;
		}
		// Цвета 0-Белый,1-Красный,2-Синий,3-Зелёный,4-Оранжевый,5-Розовый,6-Жёлтый.
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
		/// Конструктор класса UI.
		/// </summary>
		/// <param name="defaultColor">Цвет по умолчанию.</param>
		UI(UI::colors defaultColor = White) {
			if (instance != nullptr) delete this;

			instance = this;

			setlocale(LC_ALL, "Russian");
			this->defaultColor = defaultColor;
			setColor(defaultColor);
		}
		/// <summary>
		/// Задать указатель на базу данных через указатель.
		/// </summary>
		/// <param name="db">Та самая база данных.</param>
		void setDatabase(Database* db) { this->db = db; }
		/// <summary>
		/// Задать указатель на базу данных по ссылке.
		/// </summary>
		/// <param name="db">Та самая база данных.</param>
		void setDatabase(Database& db) { this->db = &db; }
		/// <summary>
		/// Цвет текста консоли по умолчанию.
		/// </summary>
		UI::colors defaultColor = White;

		/// <summary>
		/// Смена цвета текста консоли.
		/// </summary>
		/// <param name="color">Устанавливаемый цвет.</param>
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
		/// Ввод данных заданного типа.
		/// </summary>
		/// <typeparam name="T">Тип вводимых данных.</typeparam>
		/// <param name="text">Подсказка пользователю.</param>
		/// <param name="newLine">Переходить ли на следующую строку.</param>
		/// <returns>Введённые данные.</returns>
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
					printColor("&2Неправильный тип данных!");
					cin.clear();
					cin.ignore(100500, '\n');
				}
				else {
					return input;
				}
			}
		}
		/// <summary>
		/// Ввод строки без показа символов. Используется для ввода пароля.
		/// </summary>
		/// <param name="text">Подсказка пользователю.</param>
		/// <param name="newLine">Переходить ли на следующую строку.</param>
		/// <returns>Введённую строку.</returns>
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
		/// Моментальный ввод int от 0 до 9, не требует нажатия Enter. Используется в менюшках.
		/// </summary>
		/// <param name="text">Подсказка пользователю.</param>
		/// <param name="min">Левая граница.</param>
		/// <param name="max">Правая граница.</param>
		/// <param name="newLine">Переходить ли на следующую строку.</param>
		/// <returns>Число введённое пользователем.</returns>
		int inputRangeInstant(std::string text, int min = 0, int max = 9, bool newLine = true) {
			int input;
			while (true) {
				printColor(text + " : ", newLine, true);
#ifdef SOUND 
				cout << (char)7;
#endif
				input = _getch();
				if (input > 48 + max || input < min + 48) {
					printColor("&2Введите значение в диапазоне от " + to_string(min) + " до " + to_string(max));
				}
				else {
					MusicPlayer::playSound(Button_Press1);
					return input - 48;
				}
			}
		}
		/// <summary>
		/// Ввод данных в диапазоне.
		/// </summary>
		/// <typeparam name="T">Тип вводимых данных.</typeparam>
		/// <param name="text">Подсказка пользователю.</param>
		/// <param name="min">Левая граница.</param>
		/// <param name="max">Правая граница.</param>
		/// <param name="newLine">Переходить ли на следующую строку.</param>
		/// <returns>Введённые данные в диапазоне.</returns>
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
					printColor("&2Неправильный тип данных!");
					cin.clear();
					cin.ignore(100500, '\n');
				}
				else if ((max != min) && (input > max || input < min)) {
					MusicPlayer::playSound(sounds::Button_Failure);
					printColor("&2Введите значение в диапазоне от " + to_string(min) + " до " + to_string(max));
				}
				else {
					return input;
				}
			}
		}
		/// <summary>
		/// Вывести данные в консоль.
		/// </summary>
		/// <typeparam name="T">Тип выводимых данных.</typeparam>
		/// <param name="data">Данные выводиме пользователю.</param>
		/// <param name="newLine">Переходить ли на следующую строку.</param>
		template <typename T>
		void print(T data, bool newLine = true) {
			SetConsoleCP(1251);
			if (newLine)  cout << '\n';
			cout << data;
			SetConsoleCP(866);
		}
		/// <summary>
		/// Вывод строки в консоль с форматирование цвета. Флагом является символ &, номером цвета является число, следующего за ним.
		/// </summary>
		/// <param name="str">Строка для вывода.</param>
		/// <param name="newLine">Переходить ли на следующую строку.</param>
		/// <param name="animation">Задержка между печатью символов.</param>
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
		/// Вывести данные таблицей.
		/// </summary>
		/// <param name="data">Вектор строк с данными</param>
		/// <param name="precision">Оступ.</param>
		/// <param name="header">Заголовок таблицы.</param>
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
		/// Вывести заголовок.
		/// </summary>
		/// <param name="data">Текст заголовка.</param>
		/// <param name="precision">Отступ.</param>
		/// <param name="fillingChar">Символ для заполнения.</param>
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
		/// Ожидать до любого ввода символа
		/// </summary>
		void pressAnyButton() {
			cout << '\n';
			system("pause");
		}
		/// <summary>
		/// Подтверждение действия.
		/// </summary>
		/// <returns>Согласие или несогласие.</returns>
		const bool confirm() {
			return inputRangeInstant("&0Вы уверены? (&20-Нет&0,&41-Да&0)", 0, 1);
		}

		// Наборы меню	

		void adminMenu() {
			while (true) {
				if (!db->isLoggedIn()) return;
				system("cls");
				printHeader("&0Добро пожаловать " + db->getLogin(), 50);
				printColor("1 - Редактировать список объектов");
				printColor("2 - Редактировать список аккаунтов");
				printColor("0 - Выйти");
				int choice = inputRangeInstant("Выберите действие из списка", 0, 2);
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
				printHeader("&0Добро пожаловать " + db->getLogin(), 50);
				printColor("1 - Посмотреть на список объектов");
				printColor("2 - Посмотреть на список объектов по рейтингу");
				printColor("3 - Поиск в списке объектов");
				printColor("4 - Отсортировать список объектов");
				printColor("5 - Сменить пароль аккаунта");
				printColor("0 - Выйти");
				int choice = inputRangeInstant("Выберите действие из списка", 0, 5);
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
				printHeader("&0Добро пожаловать " + db->getLogin(), 50);
				printColor("1 - Посмотреть на список объектов");
				printColor("2 - Оценить объект");
				printColor("3 - Сменить пароль аккаунта");
				printColor("0 - Выйти");
				int choice = inputRangeInstant("Выберите действие из списка", 0, 3);
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
				printHeader("Редактировать список обектов", 50);
				printColor("1 - Добавить объект");
				printColor("2 - Удалить объект");
				printColor("3 - Посмотреть список объектов");
				printColor("4 - Отсортировать список объектов");
				printColor("5 - Найти объект в списке");
				printColor("0 - Выйти");
				int choice = inputRangeInstant("Выберите действие из списка", 0, 5);
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
				printHeader("Редактировать список аккаунтов", 50);
				printColor("1 - Добавить аккаунт");
				printColor("2 - Удалить аккаунт");
				printColor("3 - Посмотреть список аккаунтов");
				printColor("4 - Редактировать параметр доступа аккаунтов");
				printColor("5 - Сменить пароль аккаунта");
				printColor("0 - Выйти");
				int choice = inputRangeInstant("Выберите действие из списка", 0, 5);
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
				printHeader("Порядки сортировки", 50);
				printColor("1 - По рейтингу");
				printColor("2 - По расстоянию до метро");
				printColor("3 - По цене");
				printColor("4 - По площади");
				printColor("0 - Выйти");

				int choice = inputRangeInstant("Выберите действие из списка", 0, 5);
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
				printHeader("Признаки поиска", 50);
				printColor("1 - По названию");
				printColor("2 - По расстоянию до метро");
				printColor("0 - Выйти");
				int choice = inputRangeInstant("Выберите действие из списка", 0, 2);
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

	// Определения методов

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
			ui->printColor("&2Нет аккаунтов для авторизации");
			ui->printColor("Создайте аккаунт администратора для начала работы с базой данных");
			addAccount(1);
			currentAccount = accounts.at(0);
			accounts.at(0)->access = true;
			ui->printColor("&4Авторизация успешна");
			return;
		}
		// Предложить регистрацию
		bool isRegistered = ui->inputRangeInstant("Вы уже зарегистрированы в системе? (&20-Нет&0,&41-Да&0)", 0, 1);

		if (!isRegistered) {
			bool wantRegister = ui->inputRangeInstant("Хотите зарегистрироваться? (&20-Нет&0,&41-Да&0)", 0, 1);
			if (wantRegister) {
				addAccount();
				ui->printColor("&4Регистрация завершена, ожидайте подтверждения администратором");
			}
			else return;
		}
		while (true) {
			// Обработка исключительных ситуаций
			try {
				// Авторизация
				if (currentAccount)
					throw(AccountException(AccountExceptionType::AlreadyAuthorized, "", currentAccount->login, currentAccount->password));
				else {
					system("cls");
					ui->printColor("Войдите в аккаунт");
					string login;

					login = ui->input<string>("Введите логин");
					if (!checkLogin(login))
						throw(AccountException(AccountExceptionType::WrongLogin, "", login));

					int accountID = findID(login);
					if (!accounts.at(accountID)->access)
						throw(AccountException(AccountExceptionType::NoAccess, "", login));


					int attempts = 3;
					string password;

					do {
						password = ui->_input("Введите пароль");
						if (checkPassword(accountID, password) || attempts-- < 0) {
							break;
						}
						ui->printColor("&2Неверный пароль!");
						MusicPlayer::playSound(Button_Restricted);
					} while (true);

					if (attempts < 0)
						throw(AccountException(AccountExceptionType::WrongPassword, "&2Вы неверно ввели пароль больше 3 раз, попробуйте снова.", login, password));

					MusicPlayer::playSound(Access, true);
					MusicPlayer::playSound(Granted);

					currentAccount = accounts.at(accountID);
					ui->printColor("&4Авторизация успешна");
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
			ui->printColor("&2Список пуст");
			return;
		}
		vector<string> table;

		SetConsoleCP(1251);
		table.emplace_back("ID");
		table.emplace_back("Логин");
		table.emplace_back("Роль");
		table.emplace_back("Доступ");

		ui->printTable(table, 15, true);

		for (int i = 0; i < accounts.size(); i++) {
			table.clear();
			table.emplace_back(to_string(i));
			table.emplace_back(accounts.at(i)->login);

			switch (accounts.at(i)->accountType) {
			case 0:
				table.emplace_back("Гость");
				break;
			case 1:
				table.emplace_back("Адмирнистратор");
				break;
			case 2:
				table.emplace_back("Оценщик");
				break;
			}
			if (accounts.at(i)->access) table.emplace_back("Разрешён");
			else table.emplace_back("Запрещён");

			ui->printTable(table, 15);
		}
		SetConsoleCP(866);
	}
	void Database::addAccount(int type) {
		string login;
		while (true) {
			login = ui->input<string>("Введите логин");
			if (!checkLogin(login)) break;
			ui->printColor("&2Этот пользователь уже существует");
		}
		string password = ui->input<string>("Введите пароль");

		int accountType = type;
		if (type < 0 || type > 2)
			accountType = ui->input<int>("Введите тип аккаунта (&20-Зритель&0,&41-Администратор&0,&42-Оценщик&0)");

		if (isLoggedIn()) accounts.emplace_back(new Account(login, password, accountType, true));
		else accounts.emplace_back(new Account(login, password, accountType, false));

		MusicPlayer::playSound(Add);

		writeAccountsToFile();
	}
	void Database::removeAccount() {
		showAccounts();

		if (accounts.empty()) {
			MusicPlayer::playSound(Button_Failure);
			ui->printColor("&2 Нечего удалять");
			return;
		}
		int id = ui->inputRange<int>("Выберите номер удаляемого аккаунта", 0, accounts.size() - 1);

		if (!ui->confirm()) return;

		if (currentAccount == accounts.at(id)) {
			system("cls");
			MusicPlayer::playSound(Error);
			ui->printColor("&2Вы не можете удалить аккаунт в котором авторизованы");
			return;
		}

		delete accounts.at(id);
		accounts.erase(accounts.begin() + id);

		writeAccountsToFile();

		ui->printColor("&4Аккаунт успешно удалён");
	}
	void Database::grantAccess()
	{
		showAccounts();
		int id = ui->inputRange<int>("Выберите номер аккаунта для изменения параметра доступа", 0, accounts.size() - 1);

		if (!ui->confirm()) return;

		if (currentAccount == accounts.at(id)) {
			system("cls");
			MusicPlayer::playSound(Button_Failure, true);
			MusicPlayer::playSound(Error);
			ui->printColor("&2Вы не можете отобрать доступ у аккаунта, в котором авторизованы");
			return;
		}

		accounts.at(id)->access = !accounts.at(id)->access;
		MusicPlayer::playSound(Access, true);
		if (accounts.at(id)->access) MusicPlayer::playSound(Granted);
		else MusicPlayer::playSound(Denied);

		writeAccountsToFile();

		ui->printColor("&4Параметр доступа изменён");
	}
	void Database::changePassword()
	{
		if (!isLoggedIn()) {
			MusicPlayer::playSound(Error);
			return;
		}

		if (whoIsNow() == 1) {
			// Админ меняет пароли юзеров
			showAccounts();
			int id = ui->inputRange<int>("Выберите номер аккаунта для изменения пароля", 0, accounts.size() - 1);

			if (!ui->confirm()) return;

			string password = ui->input<string>("Введите новый пароль");
			accounts.at(id)->password = password;

			writeAccountsToFile();

			ui->printColor("&4Параметр доступа изменён");
		}
		else {
			if (!ui->confirm()) return;

			string password = ui->input<string>("Введите новый пароль");
			currentAccount->password = password;

			writeAccountsToFile();
		}
	}

	void Database::showGenericObjectInfo() {
		if (objects.empty())
		{
			MusicPlayer::playSound(Button_Failure);
			ui->printColor("&2Список объектов пуст");
			return;
		}

		vector<string> table;

		SetConsoleCP(1251);
		table.emplace_back("ID");
		table.emplace_back("Тип");
		table.emplace_back("Название");
		table.emplace_back("Площадь квм2");
		table.emplace_back("Цена");
		table.emplace_back("Метров до метро");

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
			ui->printColor("&2Список объектов пуст");
			return;
		}

		vector<string> table;

		SetConsoleCP(1251);
		table.emplace_back("ID");
		table.emplace_back("Тип");
		table.emplace_back("Название");
		table.emplace_back("Площадь квм2");
		table.emplace_back("Цена");
		table.emplace_back("Метров до метро");
		table.emplace_back("Оценка");

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
			ui->printColor("&2Список объектов пуст");
			return;
		}
		vector<string> table;

		SetConsoleCP(1251);
		table.emplace_back("ID");
		table.emplace_back("Тип");
		table.emplace_back("Название");
		table.emplace_back("Цена");
		table.emplace_back("Оценка");

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
		int objectType = ui->inputRange<int>("Введите тип недвижимости (1 - Дом, 2 - Офис)", 1, 2);
		string objectName = ui->input<string>("Введите название объекта");
		int squareMeters = ui->inputRange<int>("Введите площадь", 0, 1000);
		int metersToMetro = ui->inputRange<int>("Введите расстояние до метро", 0, 10000);

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
			ui->printColor("&2 Нечего удалять :/");
			return;
		}
		int id = ui->inputRange<int>("Выберите номер удаляемого объедка", 0, objects.size() - 1);

		if (!ui->confirm()) return;

		delete objects.at(id);
		objects.erase(objects.begin() + id);

		writeObjectsToFile();

		ui->printColor("&Доели объект и успешно удален");
	}
	void Database::rateObject()
	{
		if (objects.empty())
		{
			MusicPlayer::playSound(Button_Failure);
			ui->printColor("&2Нечего оценивать");
			return;
		}
		showGenericObjectInfo();

		int id = ui->inputRange<int>("Выберите номер оцениваемого объекта", 0, objects.size() - 1);
		int rating = ui->inputRange<int>("Введите оценку", 0, 100);

		objects.at(id)->addRating(getLogin(), rating);
		objects.at(id)->calculatePrice();

		writeObjectsToFile();
	}

	void Database::findObjectName() {
		if (objects.empty())
		{
			MusicPlayer::playSound(Button_Failure);
			ui->printColor("&2Список объектов пуст");
			return;
		}
		string objectName = ui->input<string>("Введите название для поиска: ");

		vector<string> table;

		SetConsoleCP(1251);
		table.emplace_back("ID");
		table.emplace_back("Тип");
		table.emplace_back("Название");
		table.emplace_back("Площадь квм2");
		table.emplace_back("Цена");
		table.emplace_back("Метров до метро");
		table.emplace_back("Оценка");

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
			ui->printColor("&2Список объектов пуст");
			return;
		}
		int metersToMetro = ui->input<int>("Введите расстояние для поиска: ");

		vector<string> table;

		SetConsoleCP(1251);
		table.emplace_back("ID");
		table.emplace_back("Тип");
		table.emplace_back("Название");
		table.emplace_back("Площадь квм2");
		table.emplace_back("Цена");
		table.emplace_back("Метров до метро");
		table.emplace_back("Оценка");

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
