#pragma once
#ifndef ACCOUNT
#define ACCOUNT

#include <string>
#include <fstream>

using namespace std;

namespace defaultLabStructures {
	/// <summary>
	/// Зашифровать\расшифровать строку.
	/// </summary>
	/// <param name="text">Строка к обработке.</param>
	/// <returns>Зашифрованная\расшифрованная строка.</returns>
	string enDecrypt(string text) {
		char key = text.size() % 10;

		for (char& ch : text)
			ch ^= key;
		return text;
	}
	enum AccountExceptionType {
		WrongLogin,
		WrongPassword,
		NoAccess,
		AlreadyAuthorized
	};
	/// <summary>
	/// Исключение для работ с аккаунтами.
	/// </summary>
	class AccountException : public std::exception {
	private:
		string password = "N/A";
		string login = "N/A";
		string hint = "";
		int accountType = -1;

		AccountExceptionType exceptionType;
	public:
		AccountException(AccountExceptionType exceptionType, string hint = "", string login = "N/A", string password = "N/A", int accountType = -1) {
			this->accountType = accountType;
			this->login = login;
			this->password = password;
			this->exceptionType = exceptionType;
			this->hint = hint;
		}
		const char* what() {
			string exceptionText;
			switch (exceptionType) {
			case AccountExceptionType::WrongLogin:
				exceptionText = "&2This login does not exist\n&3log: " + login + "\npassword: " + password + "\n\n" + hint;
				MusicPlayer::playSound(Error);
				return exceptionText.c_str();
				break;
			case AccountExceptionType::WrongPassword:
				exceptionText = "&2Wrong password\n&3log : " + login + "\nTried password : " + password + "\n\n" + hint;
				MusicPlayer::playSound(Button_Restricted, true);
				MusicPlayer::playSound(Access, true);
				MusicPlayer::playSound(Denied);
				return exceptionText.c_str();
				break;
			case AccountExceptionType::NoAccess:
				exceptionText = "&2This account have no access. Wait until administrator give access.\n&3log: " + login + "\npassword: " + password + "\n\n" + hint;
				MusicPlayer::playSound(Access, true);
				MusicPlayer::playSound(Denied);
				return exceptionText.c_str();
				break;
			case AccountExceptionType::AlreadyAuthorized:
				exceptionText = "&2You are already authorized.\n&3log: " + login + "\npassword: " + password + "\n\n" + hint;
				return exceptionText.c_str();
				break;
			}
			return "Custom C++ Exception";
		}
		const AccountExceptionType whatType() {
			return exceptionType;
		}
	};
	/// <summary>
	/// Учётная запись.
	/// </summary>
	struct Account {
		/// <summary>
		/// Статус учёиной записи.  0 - Зритель, 1 - Администратор, 2 - Оценщик
		/// </summary>
		int accountType = 0;
		/// <summary>
		/// Пароль учётной записи.
		/// </summary>
		string password = "";
		/// <summary>
		/// Имя учётной записи
		/// </summary>
		string login = "";
		/// <summary>
		/// Наличие доступа к баззе данных обхектов недвижимости.
		/// </summary>
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
		/// <summary>
		/// Запись в файл.
		/// </summary>
		void addToFile() const
		{
			fstream file("accounts.txt", ios::app);
			file << login << ' ' << enDecrypt(password) << ' ' << accountType << ' ' << access << '\n';
			file.close();
		}
		/// <summary>
		/// Перегруженный оператор вывода в файл.
		/// </summary>
		friend ostream& operator <<(ostream& out, Account& acc) {
			out << acc.login << ' ' << enDecrypt(acc.password) << ' ' << acc.accountType << ' ' << acc.access << '\n';
			return out;
		}
	};
}
#endif // !ACCOUNT