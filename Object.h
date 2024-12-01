#pragma once
#ifndef OBJECT
#define OBJECT

#include <string>
#include <vector>
#include <fstream>

namespace realtyPriceRate {
	/// <summary>
	/// ������ ������������.
	/// </summary>
	struct Object {
		/// <summary>
		/// ������ �������.
		/// </summary>
		struct Rating {
			/// <summary>
			/// ������� ��������������.
			/// </summary>
			int rate = 0;
			/// <summary>
			/// ����� ������� ������� ���������.
			/// </summary>
			vector<string> rated;
			/// <summary>
			/// ������.
			/// </summary>
			vector<int> ratings;
			/// <summary>
			/// ������� ������ ������������.
			/// </summary>
			/// <param name="login">��� ������� ������ ��������.</param>
			/// <param name="rating">������.</param>
			void addRating(string login, int rating) {
				for (int i = 0; i < rated.size(); i++)
					if (rated.at(i) == login) {
						ratings.at(i) = rating;
						return;
					}
				rated.emplace_back(login);
				ratings.emplace_back(rating);
			}
			/// <summary>
			/// ���������� ������� �������������� ������.
			/// </summary>
			void calculateRating() {
				if (ratings.empty()) return;
				rate = 0;
				for (int r : ratings)
					rate += r;
				rate /= ratings.size();
			}
			/// <summary>
			/// ������ � ����.
			/// </summary>
			void addToFile() const
			{
				fstream file("objects.txt", ios::app);
				for (int i = 0; i < rated.size(); i++)
					file << "r " << rated.at(i) << ' ' << ratings.at(i) << '\n';

				file.close();
			}
			/// <summary>
			/// ������������� �������� ������ � ����.
			/// </summary>
			friend ostream& operator <<(ostream& out, Rating& r)
			{
				for (int i = 0; i < r.rated.size(); i++)
					out << "r " << r.rated.at(i) << ' ' << r.ratings.at(i) << '\n';

				return out;
			}
		};
		/// <summary>
		/// ������ �������.
		/// </summary>
		Rating rating;
		/// <summary>
		/// �������� �������.
		/// </summary>
		string objectName = "";
		/// <summary>
		/// ������� � ���������� ������.
		/// </summary>
		double squareMeters = 0;
		/// <summary>
		/// ���������� ����.
		/// </summary>
		int finalPrice = 0;
		/// <summary>
		/// ���������� �� �����.
		/// </summary>
		int metersToMetro = 0;
		/// <summary>
		/// ������� �����.
		/// </summary>
		int krimeaSquareMeters = 27000;
		Object() {};
		Object(string objectName, int squareMeters, int finalPrice, int metersToMetro, int krimeaSquareMeters, int rate) {
			this->objectName = objectName;
			this->squareMeters = squareMeters;
			this->finalPrice = finalPrice;
			this->metersToMetro = metersToMetro;
			this->krimeaSquareMeters = krimeaSquareMeters;
			this->rating.rate = rate;
		}
		/// <summary>
		/// ������ � ����.
		/// </summary>
		void addToFile() const
		{
			fstream file("objects.txt", ios::app);

			file << "p " << objectName << ' ' << squareMeters << ' ' << finalPrice << ' ' << metersToMetro << ' ' << rating.rate << '\n';

			file.close();

			rating.addToFile();

		}
		/// <summary>
		/// ������������� �������� ������ � ����.
		/// </summary>
		friend ostream& operator <<(ostream& out, Object& o) {
			out << "p " << o.objectName << ' ' << o.squareMeters << ' ' << o.finalPrice << ' ' << o.metersToMetro << ' ' << o.rating.rate << '\n' << o.rating;
			return out;
		}
		/// <summary>
		/// ������ ��� ���������� �� ��������.
		/// </summary>
		static bool byRate(const Object* pt1, const Object* pt2) {
			return pt1->rating.rate < pt2->rating.rate;
		}
		/// <summary>
		/// ������ ��� ���������� �� �������.
		/// </summary>
		static bool bySquareMeters(const Object* pt1, const Object* pt2) {
			return pt1->squareMeters < pt2->squareMeters;
		}
		/// <summary>
		/// ������ ��� ���������� �� ��������� ���������.
		/// </summary>
		static bool byFinalPrice(const Object* pt1, const Object* pt2) {
			return pt1->finalPrice < pt2->finalPrice;
		}
		/// <summary>
		/// ������ ��� ���������� �� ���������� �� �����.
		/// </summary>
		static bool byMetersToMetro(const Object* pt1, const Object* pt2) {
			return pt1->metersToMetro < pt2->metersToMetro;
		}
		/// <summary>
		/// ������� ��������� ������������ ���������� ���.
		/// </summary>
		void calculatePrice() {
			finalPrice = (rating.rate * metersToMetro / 52) * krimeaSquareMeters / (squareMeters * 52);
		}
	};
}
#endif // !OBJECT