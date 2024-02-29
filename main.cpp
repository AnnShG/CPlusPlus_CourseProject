#include "main.h"
#include <regex>

using namespace std;

void addBandFun();
void showBandFun();
MusicBand band;

fstream bandFile("music_band.bin", ios::in | ios::out | ios::binary);

int menuOperations();

int main(int argc, char** argv) {

	if (!bandFile) { // if the file wasn't opened succesfully
		bandFile.open("music_band.bin", ios::in | ios::out | ios::trunc | ios::binary); // create an empty file and open for in and out
	}
	if (!bandFile) { // if still no access
		cout << "Cannot open file!" << endl;
	}
	else {
		bandFile.seekg(0, ios::end); // place the current read position to the end
		streamoff length = bandFile.tellg(); // get current position in bytes
	}

	menuOperations();

	bandFile.close();
	system("PAUSE");
	return 0;
}

int displayMenu() {
	int choice;
	cout << "What operation would you like to perform with a music band?\n\n";
	cout << Add << ". Add\n";
	cout << Show << ". Show\n";
	cout << Remove << ". Remove\n";
	cout << Search << ". Search\n";
	cout << Filter << ". Filter\n";
	cout << Sort << ". Sort\n";
	cout << Calculate << ". Calculate\n";
	cout << Exit << ". Exit\n\n";
	cout << "Enter menu choice: ";
	cin >> choice;
	cout << endl;

	while (1) {
		if (cin.fail()) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "You have entered wrong input!" << endl;
			cout << "Enter menu choice: ";
			cin >> choice;
		}
		else if (!cin.fail())
			break;
	}
	return choice;
}

int menuOperations() {
	string input;
	string searchName;
	string const byName = "name";
	string const byFoudationDate = "foundation year";
	string const numberOfRecords = "records";
	string const numberOfGenres = "genres";

	int menuChoice = displayMenu();

	while (menuChoice != Exit) {
		switch (static_cast<menu>(menuChoice)) {
		case Add:
			cout << "You chose " << Add << ". Add\n";
			addBandFun();
			cout << endl;
			break;
		case Show:
			cout << "You chose " << Show << ". Show\n\n";
			showBandFun();
			cout << endl;
			break;
		case Remove:
			cout << "You chose " << Remove << ". Remove\n\n";
			removeBandFile();
			break;
		case Search:
			cout << "You chose " << Search << ". Search\n\n";
			cout << "Enter the name of the band to search for: ";
			cin.ignore();
			getline(cin, searchName);
			searchBandByName(searchName);
			break;
		case Filter:
			cout << "You chose " << Filter << ". Filter\n\n";
			filter();
			break;
		case Sort:
			cout << "You chose " << Sort << ". Sort\n\n";
			cout << "Sort by - name | foundation year -> ";
			cin.ignore();
			getline(cin, input);
			cout << endl;

			if (input == byName) {
				sortByName();
				cout << endl;
			}
			else if (input == byFoudationDate) {
				sortByDate();
				cout << endl;
			}
			else {
				cout << "Invalid input!";
			}
			cout << endl << endl;
			break;
		case Calculate:
			cout << "You chose " << Calculate << ". Calculate\n\n";
			cout << "Would you like to calculate number of records or number of unique genres?" << endl;
			cout << "records | genres -> ";
			cin.ignore();
			getline(cin, input);
			if (input == numberOfRecords) {
				countRecords();
				cout << endl;
			}
			else if (input == numberOfGenres) {
				countUniqueGenres();
				cout << endl;
			}
			else {
				cout << "Invalid input!";
				cout << endl;
			}
			break;
		case Exit:
			cout << "You chose " << Exit << ". Exit\n\n";
			break;
		default:
			cout << "You did not make any choice!\n\n";
			break;
		}
		menuChoice = displayMenu();
	}
	return 0;
};

void addBandFun() {
	bandFile.seekg(0, ios::end);

	cout << "Add all data about music band.\n\n";
	cout << "Name is ";
	cin.ignore();

	bool isEmptyName = true;

	do {
		cin.getline(band.name, 30);

		for (int i = 0; band.name[i] != '\0'; i++) {
			if (!isspace(band.name[i])) {
				isEmptyName = false;
				break;
			}
		}

		if (isEmptyName) {
			cout << "Name cannot be empty or consist of only spaces. Please enter a valid name: ";
		}
	} while (isEmptyName);

	cout << "Foundation year is ";
	cin >> band.foundationDate;

	while (cin.fail() || band.foundationDate < 1800 || band.foundationDate > 3000) {
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "You have entered wrong input! 1800 - 3000. Example: 1989\n";
		cout << "Foundation date is ";
		cin >> band.foundationDate;
	}

	cout << "Musician name(s): ";
	cin.ignore();

	bool isEmptyMusName = true;

	do {
		cin.getline(band.musicianName, 50);

		for (int i = 0; band.musicianName[i] != '\0'; i++) {
			if (!isspace(band.musicianName[i])) {
				isEmptyMusName = false;
				break;
			}
		}

		if (isEmptyMusName) {
			cout << "This field cannot be empty or consist of only spaces. Please enter a valid musician name(s): ";
		}
	} while (isEmptyMusName);

	cout << "Genre is ";

	bool validGenre = false;
	string genreInput;

	while (!validGenre) {
		getline(cin, genreInput);

		// defined a regular expression pattern that allows only one word with letters
		regex pattern("^[a-z]+$");

		if (regex_match(genreInput, pattern)) {
			validGenre = true;
		}
		else {
			cout << "Invalid genre! Only one word with letters (no digits, punctuations, or spaces) is allowed. ";
		}
	}

	// copy the validated genre input to the band's genre field
	strcpy_s(band.genre, genreInput.c_str());
	cout << endl;

	addBand(band);
	bandFile.write((char*)&band, sizeof(band));

	cout << "Band was added successfully!" << endl;
}

void bandInfoForm(const MusicBand& band) {
	cout << left << setw(30) << "Name" << setw(20) << "Foundation Year" << setw(30) << "Musician(s)" << setw(20) << "Genre" << endl;
	cout << setfill('-') << setw(100) << "-" << setfill(' ') << endl;
}

void showBandFun() {
	int recordsCount;

	if (!bandFile) {
		cout << "Cannot open file!" << endl;
	}

	bandFile.seekg(0, ios::end);
	streamoff length = bandFile.tellg();
	recordsCount = length / sizeof(MusicBand);
	bandFile.seekg(0, ios::beg);

	cout << "All the music bands are: \n\n";
	bandInfoForm(band);

	for (int i = 0; i < recordsCount; i++) {
		bandFile.read((char*)&band, sizeof(band));
		cout << left << setw(30) << band.name << setw(20) << band.foundationDate << setw(30) << band.musicianName << setw(20) << band.genre << endl;
	}
}