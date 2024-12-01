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
	class Object {
	protected:
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
		int GOSTSquareMetersPrice = 27000;
	public:
		Object() {};
		Object(string objectName, int squareMeters, int finalPrice, int metersToMetro, int GOSTSquareMetersPrice) {
			this->objectName = objectName;
			this->squareMeters = squareMeters;
			this->finalPrice = finalPrice;
			this->metersToMetro = metersToMetro;
			this->GOSTSquareMetersPrice = GOSTSquareMetersPrice;
			this->rating.rate = 0;
		}
		/// <summary>
		/// Сеттер для рейтинга
		/// </summary>
		/// <param name="login">Имя учётной записи оценщика.</param>
		/// <param name="rating">Оценка.</param>
		virtual void addRating(string login, int rating) = 0;
		/// <summary>
		/// Запись в файл.
		/// </summary>
		virtual void addToFile() const = 0;;
		/// <summary>
		/// Перегруженный оператор вывода в файл.
		/// </summary>
		//friend ostream& operator <<(ostream& out, Object& o) 
		//{
		//	out << "p " << o.objectName << ' ' << o.squareMeters << ' ' << o.finalPrice << ' ' << o.metersToMetro << ' ' << o.rating.rate << '\n' << o.rating;
		//	return out;
		//}
		/// <summary>
		/// Лямбда для сортировки по рейтингу.
		/// </summary>
		static bool byRate(const Object* pt1, const Object* pt2)
		{
			return pt1->rating.rate < pt2->rating.rate;
		}
		/// <summary>
		/// Лямбда для сортировки по площади.
		/// </summary>
		static bool bySquareMeters(const Object* pt1, const Object* pt2)
		{
			return pt1->squareMeters < pt2->squareMeters;
		}
		/// <summary>
		/// Лямбда для сортировки по расчётной стоимости.
		/// </summary>
		static bool byFinalPrice(const Object* pt1, const Object* pt2)
		{
			return pt1->finalPrice < pt2->finalPrice;
		}
		/// <summary>
		/// Лямбда для сортировки по расстоянию до метро.
		/// </summary>
		static bool byMetersToMetro(const Object* pt1, const Object* pt2)
		{
			return pt1->metersToMetro < pt2->metersToMetro;
		}
		bool operator==(const string str) const{
			return this->objectName == str;
		}
		bool operator==(const int meters) const {
			return this->metersToMetro == meters;
		}
		/// <summary>
		/// Расчёт стоимости недвижимости происходит тут.
		/// </summary>
		virtual void calculatePrice() = 0;
		/// <summary>
		/// Поместить в контейнер общую информацию о недвижимости.
		/// </summary>
		/// <param name="row">Контейнер назначения.</param>
		virtual void getGenericInfo(vector<string>& row) = 0;
		/// <summary>
		/// Поместить в контейнер детальную информацию о недвижимости.
		/// </summary>
		/// <param name="row">Контейнер назначения.</param>
		virtual void getDetailInfo(vector<string>& row) = 0;
		/// <summary>
		/// Поместить в контейнер информацию об оценках недвижимости.
		/// </summary>
		/// <param name="row">Контейнер назначения.</param>
		virtual void getRateInfo(vector<string>& row) = 0;
	};
	class House : public Object
	{
	public:
		House() {}
		House(string objectName, int squareMeters, int finalPrice, int metersToMetro, int GOSTSquareMetersPrice) : Object(objectName, squareMeters, finalPrice, metersToMetro, GOSTSquareMetersPrice) {}
		void addRating(string login, int rating) override 
		{
			this->rating.addRating(login, rating);
		}
		void addToFile() const override
		{
			fstream file("objects.txt", ios::app);

			file << "h " << objectName << ' ' << squareMeters << ' ' << finalPrice << ' ' << metersToMetro << '\n';

			file.close();

			rating.addToFile();

		}
		void calculatePrice() override {
			rating.calculateRating();
			finalPrice = (rating.rate * metersToMetro / 52) * GOSTSquareMetersPrice / (squareMeters * 52);
		}

		void getGenericInfo(vector<string>& row) override {
			row.emplace_back("Дом");
			row.emplace_back(objectName);
			row.emplace_back(to_string(squareMeters));
			row.emplace_back(to_string(finalPrice));
			row.emplace_back(to_string(metersToMetro));
		}
		void getDetailInfo(vector<string>& row) override {
			row.emplace_back("Дом");
			row.emplace_back(objectName);
			row.emplace_back(to_string(squareMeters));
			row.emplace_back(to_string(finalPrice));
			row.emplace_back(to_string(metersToMetro));
			row.emplace_back(to_string(rating.rate));
		}
		void getRateInfo(vector<string>& row) override {
			row.emplace_back("Дом");
			row.emplace_back(objectName);
			row.emplace_back(to_string(finalPrice));
			row.emplace_back(to_string(rating.rate));
		}
	};
	class Office : public Object
	{
	public:
		Office() {}
		Office(string objectName, int squareMeters, int finalPrice, int metersToMetro, int GOSTSquareMetersPrice) : Object(objectName, squareMeters, finalPrice, metersToMetro, GOSTSquareMetersPrice) {}
		void addRating(string login, int rating) override
		{
			this->rating.addRating(login, rating);
		}
		void addToFile() const override
		{
			fstream file("objects.txt", ios::app);

			file << "o " << objectName << ' ' << squareMeters << ' ' << finalPrice << ' ' << metersToMetro << '\n';

			file.close();

			rating.addToFile();

		}
		void calculatePrice() override {
			rating.calculateRating();
			finalPrice = (rating.rate/100) * (metersToMetro + squareMeters) / (GOSTSquareMetersPrice / 5252);
		}

		void getGenericInfo(vector<string>& row) override {
			row.emplace_back("Офис");
			row.emplace_back(objectName);
			row.emplace_back(to_string(squareMeters));
			row.emplace_back(to_string(finalPrice) + "$");
			row.emplace_back(to_string(metersToMetro));
		}
		void getDetailInfo(vector<string>& row) override {
			row.emplace_back("Офис");
			row.emplace_back(objectName);
			row.emplace_back(to_string(squareMeters));
			row.emplace_back(to_string(finalPrice) + "$");
			row.emplace_back(to_string(metersToMetro));
			row.emplace_back(to_string(rating.rate));
		}
		void getRateInfo(vector<string>& row) override {
			row.emplace_back("Офис");
			row.emplace_back(objectName);
			row.emplace_back(to_string(finalPrice) + "$");
			row.emplace_back(to_string(rating.rate));
		}
	};
}
#endif // !OBJECT