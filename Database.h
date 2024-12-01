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
	/// ����� ���� ������.
	/// </summary>
	class Database {
	private:
		/// <summary>
		/// �������������� � ������ ������ ������� ������.
		/// </summary>
		Account* currentAccount = nullptr;
		/// <summary>
		/// ���� ������� �������.
		/// </summary>
		vector<Account*> accounts;
		/// <summary>
		/// ���� �������� ������������.
		/// </summary>
		vector<Object*> objects;
		/// <summary>
		/// ��������� �� ���������.
		/// </summary>
		UI* ui;
		/// <summary>
		/// ����� ����� �� ������ � ���� ������� �������.
		/// </summary>
		/// <param name="login">��� ������� ������.</param>
		/// <returns>����� �� ������ � ���� ������� �������.</returns>
		int findID(string login) {
			for (int i = 0; i < accounts.size(); i++) {
				if (accounts.at(i)->login == login) return i;
			}
			return -1;
		}
		/// <summary>
		/// ��������� ������.
		/// </summary>
		/// <param name="id">����� �� ������ � ���� ������� �������.</param>
		/// <param name="password">�������������� ������.</param>
		/// <returns>������� ������ ��� ���.</returns>
		const bool checkPassword(int id, string password) {
			return accounts.at(id)->password == password;
		}
		/// <summary>
		/// ������� ��� ������� ������������
		/// </summary>
		void calculateObjectsRatings();

	public:
		Database();

		~Database();

		/// <summary>
		/// ��������� ������ �������� ��������.
		/// </summary>
		/// <returns>������ ������������.</returns>
		const int whoIsNow() const {
			if (isLoggedIn()) return currentAccount->accountType;
			else return -1;
		}
		/// <summary>
		/// �������� ������ ������������.
		/// </summary>
		/// <param name="login">��� ������������</param>
		/// <returns>������ ������������.</returns>
		const int whoIs(string login) const {
			for (Account* ac : accounts) {
				if (ac->login == login) return ac->accountType;
			}
			return -1;
		}
		/// <summary>
		/// ��������� �� ������ ���� � ���������.
		/// </summary>
		/// <returns>�� - 1, ��� - 0.</returns>
		const bool isLoggedIn() const { return currentAccount; }
		/// <summary>
		/// ���� �� ��������� � ������ ������ ������������.
		/// </summary>
		/// <param name="login">��� ������������.</param>
		/// <returns>�� - 1, ��� - 0.</returns>
		const bool checkLogin(string login) {
			for (Account* ac : accounts) {
				if (ac->login == login) return true;
			}
			return false;
		}
		/// <summary>
		/// ����� �� ���� ������.
		/// </summary>
		/// <returns>�� - 1, ��� - 0.</returns>
		const bool isEmpty() { return objects.empty(); }
		/// <summary>
		/// �������� ��� �������� ������������.
		/// </summary>
		/// <returns>��� ������� ������������.</returns>
		const string getLogin() const { return currentAccount->login; }

		// ������� ������ //
		
		/// <summary>
		/// ���� �����������.
		/// </summary>
		void login();
		/// <summary>
		/// �������������� ������������.
		/// </summary>
		void deauthorize() { currentAccount = nullptr; }
		/// <summary>
		/// ������� ������ ������� �������.
		/// </summary>
		void showAccounts();
		/// <summary>
		/// �������� ������� ������ � ����.
		/// </summary>
		/// <param name="type">������ ��������.</param>
		void addAccount(int type = -1);
		/// <summary>
		/// ������� ������� ������ �� ����.
		/// </summary>
		void removeAccount();
		/// <summary>
		/// ������������ ������ � ���� ��������.
		/// </summary>
		void grantAccess();
		/// <summary>
		/// ������� ������.
		/// </summary>
		void changePassword();

		/// <summary>
		/// �������� �������� � ����
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
		/// ��������� �������� �� �����.
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

		// ��������� //

		/// <summary>
		/// �������� ����� ���������� �� ��������.
		/// </summary>
		void showGenericObjectInfo();
		/// <summary>
		/// �������� ������ ���������� �� ��������.
		/// </summary>
		void showDetailObjectInfo();
		/// <summary>
		/// �������� ���������� � ���� ��������.
		/// </summary>
		void showRateObjectInfo();

		/// <summary>
		/// �������� ������ � ���� ������.
		/// </summary>
		void addObject();
		/// <summary>
		/// ������� ������� �� ���� ������.
		/// </summary>
		void removeObject();
		/// <summary>
		/// ������ �������.
		/// </summary>
		void rateObject();
		/// <summary>
		/// ������ ��� ���������� �� ��������.
		/// </summary>
		void sortObjectRate() { sort(objects.begin(), objects.end(), Object::byRate); }
		/// <summary>
		/// ������ ��� ���������� �� ���������� ���������� ������.
		/// </summary>
		void sortObjectSquareMeters() { sort(objects.begin(), objects.end(), Object::bySquareMeters); }
		/// <summary>
		/// ������ ��� ���������� �� ��������� ����.
		/// </summary>
		void sortObjectFinalPrice() { sort(objects.begin(), objects.end(), Object::byFinalPrice); }
		/// <summary>
		/// ������ ��� ���������� �� ���������� �� �����.
		/// </summary>
		void sortObjectMetersToMetro() { sort(objects.begin(), objects.end(), Object::byMetersToMetro); }
		/// <summary>
		/// ����� ������� �� ��������.
		/// </summary>
		void findObjectName();
		/// <summary>
		/// ����� ������� �� ���������� �� �����.
		/// </summary>
		void findObjectMetersToMetro();

		/// <summary>
		/// �������� ������� ������������ � ����.
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
		/// ��������� ������� �� �����.
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
