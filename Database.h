#pragma once
#ifndef DATABASE
#define DATABASE

#include "Music.h"
#include "Account.h"
#include "Object.h"
#include <fstream>
#include <algorithm>

class UI;

class Database {
private:
	Account* currentAccount = nullptr;
	vector<Account*> accounts;

	vector<Object*> objects;

	UI* ui;

	int findID(string login) {
		for (int i = 0; i < accounts.size(); i++) {
			if (accounts.at(i)->login == login) return i;
		}
		return -1;
	}
	const bool checkPassword(int id, string password) {
		return accounts.at(id)->password == password;
	}

	void calculateObjectsRatings();

public:
	Database();

	~Database();

	const int whoIsNow() const {
		if (isLoggedIn()) return currentAccount->accountType;
		else return -1;
	}
	const int whoIs(string login) const {
		for (Account* ac : accounts) {
			if (ac->login == login) return ac->accountType;
		}
		return -1;
	}
	const bool isLoggedIn() const { return currentAccount; }

	const bool checkLogin(string login) {
		for (Account* ac : accounts) {
			if (ac->login == login) return true;
		}
		return false;
	}

	const bool isEmpty() { return objects.empty(); }

	const string getLogin() const { return currentAccount->login; }

	// Учётные записи //
	void login();
	void deauthorize() { currentAccount = nullptr; }

	void showAccounts();
	void addAccount(int type = -1);
	void removeAccount();
	void grantAccess();
	void changePassword();

	void writeAccountsToFile()
	{
		fstream file("accounts.txt", ios::out);
		file << accounts.size() << '\n';
		file.close();

		for (int i = 0; i < accounts.size(); i++)
			accounts.at(i)->addToFile();
	}
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
	void showGenericObjectInfo();
	void showDetailObjectInfo();
	void showRateObjectInfo();

	void addObject();
	void removeObject();
	void rateObject();

	void sortObjectRate() { sort(objects.begin(), objects.end(), Object::byRate); }
	void sortObjectSquareMeters() { sort(objects.begin(), objects.end(), Object::bySquareMeters); }
	void sortObjectFinalPrice() { sort(objects.begin(), objects.end(), Object::byFinalPrice); }
	void sortObjectMetersToMetro() { sort(objects.begin(), objects.end(), Object::byMetersToMetro); }

	void findObjectName();
	void findObjectMetersToMetro();

	void writeObjectsToFile()
	{
		fstream file("objects.txt", ios::out);
		file << objects.size() << '\n';
		file.close();

		for (int i = 0; i < objects.size(); i++)
			objects.at(i)->addToFile();
	}
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
			if (temp == "p") {
				i++;
				string objectName;
				int squareMeters, finalPrice, rate, metersToMetro;
				getline(file, objectName, ' ');
				getline(file, temp, ' ');
				squareMeters = stoi(temp);
				getline(file, temp, ' ');
				finalPrice = stoi(temp);
				getline(file, temp, ' ');
				metersToMetro = stoi(temp);
				getline(file, temp, '\n');
				rate = stoi(temp);
				objects.emplace_back(new Object(objectName,squareMeters,finalPrice,metersToMetro,27000,rate));
			}
			else if (temp == "r") {
				string login;
				int rating;
				getline(file, login, ' ');
				getline(file, temp, '\n');
				rating = stoi(temp);
				objects.at(objects.size() - 1)->rating.addRating(login, rating);
			}
			objects[i-1]->calculatePrice();
		}

		file.close();
	}
};

#endif // !DATABASE
