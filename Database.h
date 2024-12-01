#pragma once
#ifndef DATABASE
#define DATABASE

#include "Account.h"
#include "Object.h"
#include <fstream>
#include <algorithm>

using namespace defaultLabStructures;

namespace realtyPriceRate {
	class UI;
	/// <summary>
	/// Класс базы данных.
	/// </summary>
	class Database {
	private:
		/// <summary>
		/// Авторизованная в данный момент учётная запись.
		/// </summary>
		Account* currentAccount = nullptr;
		/// <summary>
		/// База учётных записей.
		/// </summary>
		vector<Account*> accounts;
		/// <summary>
		/// База объектов недвижимости.
		/// </summary>
		vector<Object*> objects;
		/// <summary>
		/// Указатель на интерфейс.
		/// </summary>
		UI* ui;
		/// <summary>
		/// Найди номер уч записи в базе учётных записей.
		/// </summary>
		/// <param name="login">Имя учётной записи.</param>
		/// <returns>Номер уч записи в базе учётных записей.</returns>
		int findID(string login) {
			for (int i = 0; i < accounts.size(); i++) {
				if (accounts.at(i)->login == login) return i;
			}
			return -1;
		}
		/// <summary>
		/// Проверить пароль.
		/// </summary>
		/// <param name="id">Номер уч записи в базе учётных записей.</param>
		/// <param name="password">Предполагаемый пароль.</param>
		/// <returns>Подошёл пароль или нет.</returns>
		const bool checkPassword(int id, string password) {
			return accounts.at(id)->password == password;
		}
		/// <summary>
		/// Оценить все объекты недвижимости
		/// </summary>
		void calculateObjectsRatings();

	public:
		Database();

		~Database();

		/// <summary>
		/// Проверить статус текущего аккаунта.
		/// </summary>
		/// <returns>Статус пользователя.</returns>
		const int whoIsNow() const {
			if (isLoggedIn()) return currentAccount->accountType;
			else return -1;
		}
		/// <summary>
		/// Получить статус пользователя.
		/// </summary>
		/// <param name="login">Имя пользователя</param>
		/// <returns>Статус пользователя.</returns>
		const int whoIs(string login) const {
			for (Account* ac : accounts) {
				if (ac->login == login) return ac->accountType;
			}
			return -1;
		}
		/// <summary>
		/// Произведён ли сейчас вход в программу.
		/// </summary>
		/// <returns>Да - 1, нет - 0.</returns>
		const bool isLoggedIn() const { return currentAccount; }
		/// <summary>
		/// Есть ли аккаунтом с данным именем пользователя.
		/// </summary>
		/// <param name="login">Имя пользователя.</param>
		/// <returns>Да - 1, нет - 0.</returns>
		const bool checkLogin(string login) {
			for (Account* ac : accounts) {
				if (ac->login == login) return true;
			}
			return false;
		}
		/// <summary>
		/// Пуста ли база данных.
		/// </summary>
		/// <returns>Да - 1, нет - 0.</returns>
		const bool isEmpty() { return objects.empty(); }
		/// <summary>
		/// Получить имя текущего пользователя.
		/// </summary>
		/// <returns>Имя екущего пользователя.</returns>
		const string getLogin() const { return currentAccount->login; }

		// Учётные записи //
		
		/// <summary>
		/// Цикл авторизации.
		/// </summary>
		void login();
		/// <summary>
		/// Деавторизовать пользователя.
		/// </summary>
		void deauthorize() { currentAccount = nullptr; }
		/// <summary>
		/// Вывести список учётных записей.
		/// </summary>
		void showAccounts();
		/// <summary>
		/// Добавить учётную запись в базу.
		/// </summary>
		/// <param name="type">Статус аккаунта.</param>
		void addAccount(int type = -1);
		/// <summary>
		/// Удалить учётную запись из базы.
		/// </summary>
		void removeAccount();
		/// <summary>
		/// Предоставить доступ к базе объектов.
		/// </summary>
		void grantAccess();
		/// <summary>
		/// Сменить пароль.
		/// </summary>
		void changePassword();

		/// <summary>
		/// Записать аккаунты в файл
		/// </summary>
		void writeAccountsToFile()
		{
			fstream file("accounts.txt", ios::out);
			file << accounts.size() << '\n';
			file.close();

			for (int i = 0; i < accounts.size(); i++)
				accounts.at(i)->addToFile();
		}
		/// <summary>
		/// Прочитать аккаунты из файла.
		/// </summary>
		void readAccountsFromFile()
		{
			accounts.clear();
			fstream file("accounts.txt", ios::in);

			if (file.eof() || !file.is_open())
			{
				file.close();
				return;
			}

			string temp;

			getline(file, temp, '\n');
			int size = stoi(temp);

			for (int i = 0; i < size; i++)
			{
				string login, password;
				getline(file, login, ' ');
				getline(file, password, ' ');

				getline(file, temp, ' ');
				int accountType = stoi(temp);

				getline(file, temp, '\n');
				bool access = stoi(temp);

				accounts.emplace_back(new Account(login, enDecrypt(password), accountType, access));

			}

			file.close();
		}

		// Участники //

		/// <summary>
		/// Показать общую информацию об объектах.
		/// </summary>
		void showGenericObjectInfo();
		/// <summary>
		/// Показать полную информацию об объектах.
		/// </summary>
		void showDetailObjectInfo();
		/// <summary>
		/// Показать информацию о цене объектов.
		/// </summary>
		void showRateObjectInfo();

		/// <summary>
		/// Добавить обхект в базу данных.
		/// </summary>
		void addObject();
		/// <summary>
		/// Удалить объедок из дазы банных.
		/// </summary>
		void removeObject();
		/// <summary>
		/// Оценка объекта.
		/// </summary>
		void rateObject();
		/// <summary>
		/// Лямбда для сортировки по рейтингу.
		/// </summary>
		void sortObjectRate() { sort(objects.begin(), objects.end(), Object::byRate); }
		/// <summary>
		/// Лямбда для сортировки по количеству квадратных метров.
		/// </summary>
		void sortObjectSquareMeters() { sort(objects.begin(), objects.end(), Object::bySquareMeters); }
		/// <summary>
		/// Лямбда для сортировки по расчётной цене.
		/// </summary>
		void sortObjectFinalPrice() { sort(objects.begin(), objects.end(), Object::byFinalPrice); }
		/// <summary>
		/// Лямбда для сортировки по расстоянию до метро.
		/// </summary>
		void sortObjectMetersToMetro() { sort(objects.begin(), objects.end(), Object::byMetersToMetro); }
		/// <summary>
		/// Поиск объекта по названию.
		/// </summary>
		void findObjectName();
		/// <summary>
		/// Поиск обхекта по расстоянию до метро.
		/// </summary>
		void findObjectMetersToMetro();

		/// <summary>
		/// Записать объекты недвижимости в файл.
		/// </summary>
		void writeObjectsToFile()
		{
			fstream file("objects.txt", ios::out);
			file << objects.size() << '\n';
			file.close();

			for (int i = 0; i < objects.size(); i++)
				objects.at(i)->addToFile();

		}
		/// <summary>
		/// Прочитать объекты из файла.
		/// </summary>
		void readObjectsFromFile()
		{
			objects.clear();
			fstream file("objects.txt", ios::in);

			if (file.eof() || !file.is_open())
			{
				file.close();
				return;
			}

			string temp;

			getline(file, temp, '\n');
			int size = stoi(temp);

			for (int i = 0; i < size; )
			{
				getline(file, temp, ' ');
				if (temp == "h" || temp == "o") {
					i++;
					string objectName,objectType = temp;
					int squareMeters, finalPrice, rate, metersToMetro;
					getline(file, objectName, ' ');
					getline(file, temp, ' ');
					squareMeters = stoi(temp);
					getline(file, temp, ' ');
					finalPrice = stoi(temp);
					getline(file, temp, '\n');
					metersToMetro = stoi(temp);
					if (objectType == "h")
						objects.emplace_back(new House(objectName, squareMeters, finalPrice, metersToMetro, 27000));
					else if (objectType == "o")
						objects.emplace_back(new Office(objectName, squareMeters, finalPrice, metersToMetro, 27000));
				}
				else if (temp == "r") {
					string login;
					int rating;
					getline(file, login, ' ');
					getline(file, temp, '\n');
					rating = stoi(temp);
					objects.at(objects.size() - 1)->addRating(login, rating);

					objects[i - 1]->calculatePrice();
				}
			}

			file.close();
		}
	};
}
#endif // !DATABASE
