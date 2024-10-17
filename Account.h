#pragma once
#ifndef ACCOUNT
#define ACCOUNT

#include <string>
#include <fstream>

using namespace std;

string enDecrypt(string text) {
	char key = text.size() % 10;

	for (char& ch : text)
		ch ^= key;
	return text;
}

struct Account {
	// 0 - �������, 1 - �������������, 2 - �������
	int accountType = 0;
	string password = "";
	string login = "";
	bool access = false;
	Account() {};
	Account(string login, string password, int accountType, bool access) {
		this->login = login;
		this->password = password;
		this->accountType = accountType;
		this->access = access;
	}
	bool operator == (const Account& account) {
		return login == account.login && password == account.password;
	}
	void addToFile() const
	{
		fstream file("accounts.txt", ios::app);
		file << login << ' ' << enDecrypt(password) << ' ' << accountType << ' '<< access <<'\n';
		file.close();
	}
};
#endif // !ACCOUNT