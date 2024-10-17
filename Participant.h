#pragma once
#ifndef PARTICIPANT
#define PARTICIPANT

#include <string>
#include <vector>
#include <fstream>

struct Object {
	struct Rating {
		int rate = 0;
		vector<string> rated;
		vector<int> ratings;

		void addRating(string login, int rating) {
			for (int i = 0; i < rated.size(); i++)
				if (rated.at(i) == login) {
					ratings.at(i) = rating;
					return;
				}
			rated.emplace_back(login);
			ratings.emplace_back(rating);
		}
		void calculateRating() {
			if (ratings.empty()) return;
			rate = 0;
			for (int r : ratings)
				rate += r;
			rate /= ratings.size();
		}

		void addToFile() const
		{
			fstream file("objects.txt", ios::app);
			for (int i = 0; i < rated.size(); i++)
				file << "r " << rated.at(i) << ' ' << ratings.at(i) << '\n';

			file.close();
		}
		friend ostream& operator <<(ostream& out, Rating& r)
		{
			for (int i = 0; i < r.rated.size(); i++)
				out << "r " << r.rated.at(i) << ' ' << r.ratings.at(i) << '\n';

			return out;
		}
	};

	Rating rating;

	string objectName = "";
	double squareMeters = 0;
	int finalPrice = 0;
	int metersToMetro = 0;
	int krimeaSquareMeters = 27000;

	bool isParticipant = true;
	Object() {};
	Object(string objectName, int squareMeters, int finalPrice, int metersToMetro, int krimeaSquareMeters, int rate) {
		this->objectName = objectName;
		this->squareMeters = squareMeters;
		this->finalPrice = finalPrice;
		this->metersToMetro = metersToMetro;
		this->krimeaSquareMeters = krimeaSquareMeters;
		this->rating.rate = rate;
	}
	void addToFile() const
	{
		fstream file("objects.txt", ios::app);

		file << "p " << objectName << ' ' << squareMeters << ' ' << finalPrice << ' ' << metersToMetro << ' ' << rating.rate << '\n';

		rating.addToFile();

		file.close();
	}
	friend ostream& operator <<(ostream& out, Object& o) {
		out << "p " << o.objectName << ' ' << o.squareMeters << ' ' << o.finalPrice << ' ' << o.metersToMetro << ' ' << o.rating.rate << '\n' << o.rating;
		return out;
	}
	static bool byRate(const Object* pt1, const Object* pt2) {
		return pt1->rating.rate < pt2->rating.rate;
	}
	static bool bySquareMeters(const Object* pt1, const Object* pt2) {
		return pt1->squareMeters < pt2->squareMeters;
	}
	static bool byFinalPrice(const Object* pt1, const Object* pt2) {
		return pt1->finalPrice < pt2->finalPrice;
	}
	static bool byMetersToMetro(const Object* pt1, const Object* pt2) {
		return pt1->metersToMetro < pt2->metersToMetro;
	}

	void calculatePrice() {
		finalPrice = (rating.rate * metersToMetro / 52) * krimeaSquareMeters / (squareMeters * 52 * 7);
	}
};
#endif // !PARTICIPANT