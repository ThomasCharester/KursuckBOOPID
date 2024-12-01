#pragma once
#ifndef OBJECT
#define OBJECT

#include <string>
#include <vector>
#include <fstream>

namespace realtyPriceRate {
	/// <summary>
	/// Объект недвижимости.
	/// </summary>
	struct Object {
		/// <summary>
		/// Оценки объекта.
		/// </summary>
		struct Rating {
			/// <summary>
			/// Средняя арифметическая.
			/// </summary>
			int rate = 0;
			/// <summary>
			/// Имена учётных записей оценщиков.
			/// </summary>
			vector<string> rated;
			/// <summary>
			/// Оценки.
			/// </summary>
			vector<int> ratings;
			/// <summary>
			/// Оценить объект недвижимости.
			/// </summary>
			/// <param name="login">Имя учётной записи оценщика.</param>
			/// <param name="rating">Оценка.</param>
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
			/// Рассчитать среднюю арифметическую оценку.
			/// </summary>
			void calculateRating() {
				if (ratings.empty()) return;
				rate = 0;
				for (int r : ratings)
					rate += r;
				rate /= ratings.size();
			}
			/// <summary>
			/// Запись в файл.
			/// </summary>
			void addToFile() const
			{
				fstream file("objects.txt", ios::app);
				for (int i = 0; i < rated.size(); i++)
					file << "r " << rated.at(i) << ' ' << ratings.at(i) << '\n';

				file.close();
			}
			/// <summary>
			/// Перегруженный оператор вывода в файл.
			/// </summary>
			friend ostream& operator <<(ostream& out, Rating& r)
			{
				for (int i = 0; i < r.rated.size(); i++)
					out << "r " << r.rated.at(i) << ' ' << r.ratings.at(i) << '\n';

				return out;
			}
		};
		/// <summary>
		/// Оценки объекта.
		/// </summary>
		Rating rating;
		/// <summary>
		/// Название объекта.
		/// </summary>
		string objectName = "";
		/// <summary>
		/// Площать в квадратных метрах.
		/// </summary>
		double squareMeters = 0;
		/// <summary>
		/// Рассчётная цена.
		/// </summary>
		int finalPrice = 0;
		/// <summary>
		/// Расстояние до метро.
		/// </summary>
		int metersToMetro = 0;
		/// <summary>
		/// Площадь Крыма.
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
		/// Запись в файл.
		/// </summary>
		void addToFile() const
		{
			fstream file("objects.txt", ios::app);

			file << "p " << objectName << ' ' << squareMeters << ' ' << finalPrice << ' ' << metersToMetro << ' ' << rating.rate << '\n';

			file.close();

			rating.addToFile();

		}
		/// <summary>
		/// Перегруженный оператор вывода в файл.
		/// </summary>
		friend ostream& operator <<(ostream& out, Object& o) {
			out << "p " << o.objectName << ' ' << o.squareMeters << ' ' << o.finalPrice << ' ' << o.metersToMetro << ' ' << o.rating.rate << '\n' << o.rating;
			return out;
		}
		/// <summary>
		/// Лямбда для сортировки по рейтингу.
		/// </summary>
		static bool byRate(const Object* pt1, const Object* pt2) {
			return pt1->rating.rate < pt2->rating.rate;
		}
		/// <summary>
		/// Лямбда для сортировки по площади.
		/// </summary>
		static bool bySquareMeters(const Object* pt1, const Object* pt2) {
			return pt1->squareMeters < pt2->squareMeters;
		}
		/// <summary>
		/// Лямбда для сортировки по расчётной стоимости.
		/// </summary>
		static bool byFinalPrice(const Object* pt1, const Object* pt2) {
			return pt1->finalPrice < pt2->finalPrice;
		}
		/// <summary>
		/// Лямбда для сортировки по расстоянию до метро.
		/// </summary>
		static bool byMetersToMetro(const Object* pt1, const Object* pt2) {
			return pt1->metersToMetro < pt2->metersToMetro;
		}
		/// <summary>
		/// Рассчёт стоимости недвижимости происходит тут.
		/// </summary>
		void calculatePrice() {
			finalPrice = (rating.rate * metersToMetro / 52) * krimeaSquareMeters / (squareMeters * 52);
		}
	};
}
#endif // !OBJECT