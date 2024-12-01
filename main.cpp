#include "UserInterface.h"

using namespace realtyPriceRate;

int main() {

	//MusicPlayer player;
	//player.startPlaying();

	MusicPlayer::playMusic();

	UI ui(UI::colors::Pink);
	Database database;

	ui.setDatabase(&database);

	while (true) {
		system("cls");

		database.login();

		switch (database.whoIsNow()) {
		case 0:
			ui.spectatorMenu();
			break;
		case 1:
			ui.adminMenu();
			break;
		case 2:
			ui.judgeMenu();
			break;
		}
		bool choice = ui.inputRangeInstant("������� &2�����&0 ��� &4���������� &9������������� ���������? (&20-�����&0/&41-����������&0)",0,1);

		if (!choice) {
			MusicPlayer::playSound(Exit1);
			if (!ui.confirm()) continue; /// XD
			ui.printColor("&6���������� ��������");
			return 0;
		}
	}

	return 0;
}


