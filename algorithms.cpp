#include <algorithm>
#include <vector>
#include "main.h"

using namespace std;

vector<MusicBand> bands;

void addBand(const MusicBand& b) {
	bands.push_back(b);
}

void removeBandByName(vector<MusicBand>& bands, const string& bandName) {
	/* lambda f - n checks if entered name is equal to any name attribute in MusicBand object in the vector
		if it is so, the lambda return true and such an element should be removed*/
	auto it = remove_if(bands.begin(), bands.end(), [&](const MusicBand& b) {
		return b.name == bandName;
	});

	if (it != bands.end()) {
		bands.erase(it, bands.end());
		cout << "The band '" << bandName << "' removed successfully!\n\n";
	}
	else {
		cout << "No band with the name '" << bandName << "' found.\n\n";
	}
}

void removeBandFile() {
	bands.clear(); // clear all the elements from the vector ssso there was only up to date data

	MusicBand band;
	ifstream bandFile("music_band.bin", ios::binary);

	if (!bandFile) {
		cout << "Error opening file!" << endl;
		return;
	}

	// push each record into the bands vector (from the bin file)
	while (bandFile.read(reinterpret_cast<char*>(&band), sizeof(band))) {
		bands.push_back(band);
	}

	bandFile.close();

	string bandNameToRemove;
	cout << "Enter the name of the band to remove: ";
	cin.ignore();
	getline(cin, bandNameToRemove);

	removeBandByName(bands, bandNameToRemove);

	// clear the file
	ofstream outFile("music_band.bin", ios::binary | ios::trunc);
	outFile.close();

	// write updated bands back to the binary file
	ofstream appendFile("music_band.bin", ios::binary | ios::app);
	for (const MusicBand& b : bands) {
		appendFile.write(reinterpret_cast<const char*>(&b), sizeof(b));
	}

	appendFile.close();
}

void displayFilterByCriterion(const vector<MusicBand>& bands, const string& criterion) {
	if (criterion == "name") {
		cout << left << setw(30) << "Name" << setw(20) << endl;
		cout << setfill('-') << setw(30) << "-" << setfill(' ') << endl;
		for (const MusicBand& band : bands) {
			cout << left << setw(30) << band.name << setw(20) << endl;
		}
	}
	else if (criterion == "musician") {
		cout << left << setw(30) << "Musician(s)" << setw(20) << endl;
		cout << setfill('-') << setw(30) << "-" << setfill(' ') << endl;
		for (const MusicBand& band : bands) {
			cout << left << setw(30) << band.musicianName << setw(20) << endl;
		}
	}
	else if (criterion == "genre") {
		cout << left << setw(30) << "Genre" << setw(20) << endl;
		cout << setfill('-') << setw(30) << "-" << setfill(' ') << endl;
		for (const MusicBand& band : bands) {
			cout << left << setw(30) << band.genre << setw(20) << endl;
		}
	}
	cout << endl;
}

void filter() {
	bands.clear();
	MusicBand band;

	ifstream bandFile("music_band.bin", ios::binary);

	if (!bandFile) {
		cout << "Error opening file!" << endl;
	}

	while (bandFile.read(reinterpret_cast<char*>(&band), sizeof(band))) {
		bands.push_back(band);
	}
	bandFile.close();

	int choice;
	cin.ignore();
	cout << "Choose a criterion to filter by:" << endl;
	cout << "1. Band Name" << endl;
	cout << "2. Musician Name" << endl;
	cout << "3. Genre" << endl << endl;
	cout << "Your criterion -> ";
	cin >> choice;

	while (1) {
		if (cin.fail()) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "You have entered wrong input!" << endl;
			cout << "Your criterion -> ";
			cin >> choice;
		}
		else if (!cin.fail())
			break;
	}
	cout << endl;

	string criterion;
	if (choice == 1) {
		criterion = "name";
	}
	else if (choice == 2) {
		criterion = "musician";
	}
	else if (choice == 3) {
		criterion = "genre";
	}
	else {
		cout << "Invalid choice!\n\n";
	}

	displayFilterByCriterion(bands, criterion);
}

void sortByDateOrder() {
	sort(bands.begin(), bands.end(), [](const MusicBand& a, const MusicBand& b) {
		return a.foundationDate < b.foundationDate;
	});
}

void sortByDate() {
	MusicBand band;
	bands.clear();

	ifstream bandFile("music_band.bin", ios::binary);

	if (!bandFile) {
		cout << "Error opening file!" << endl;
	}

	while (bandFile.read(reinterpret_cast<char*>(&band), sizeof(band))) {
		bands.push_back(band);
	}
	bandFile.close();

	sortByDateOrder();
	bandInfoForm(band);

	for (const MusicBand& sortedBand : bands) {
		cout << left << setw(30) << sortedBand.name << setw(20) << sortedBand.foundationDate << setw(30) << sortedBand.musicianName << setw(20) << sortedBand.genre << endl;
	}
}

void sortByFirstLetter() {
	for (size_t i = 0; i < bands.size() - 1; i++) {
		for (size_t j = 0; j < bands.size() - i - 1; j++) {
			if (bands[j].name[0] > bands[j + 1].name[0]) {
				swap(bands[j], bands[j + 1]);
			}
		}
	}
}

void sortByName() {
	MusicBand band;
	bands.clear();

	ifstream bandFile("music_band.bin", ios::binary);

	if (!bandFile) {
		cout << "Error opening file!" << endl;
	}

	while (bandFile.read(reinterpret_cast<char*>(&band), sizeof(band))) {
		bands.push_back(band);
	}
	bandFile.close();

	sortByFirstLetter();
	bandInfoForm(band);
	for (const MusicBand& sortedBand : bands) {
		cout << left << setw(30) << sortedBand.name << setw(20) << sortedBand.foundationDate << setw(30) << sortedBand.musicianName << setw(20) << sortedBand.genre << endl;
	}
}

int countRecords() {
	ifstream bandFile("music_band.bin", ios::binary);

	if (!bandFile) {
		cout << "Error opening file!" << endl;
	}

	bandFile.seekg(0, ios::end);
	streamoff length = bandFile.tellg();
	int recordsCount = length / sizeof(MusicBand);

	bandFile.close();
	cout << "Records are " << recordsCount << endl;
	cout << "Length of the file is " << length << " bytes\n\n";
	return recordsCount;
}

int countUniqueGenres() {
	vector<string> uniqueGenres;
	ifstream bandFile("music_band.bin", ios::binary);

	if (!bandFile) {
		cout << "Error opening file!" << endl;
		return -1;
	}

	MusicBand band;
	while (bandFile.read(reinterpret_cast<char*>(&band), sizeof(band))) {
		// Check if the genre is already in the uniqueGenres vector
		if (find(uniqueGenres.begin(), uniqueGenres.end(), band.genre) == uniqueGenres.end()) {
			uniqueGenres.push_back(band.genre);
		}
	}

	bandFile.close();
	cout << "Number of unique genres are " << uniqueGenres.size() << endl;
	cout << "They are: ";
	for (const string& genre : uniqueGenres) {
		cout << genre << ", ";
	}

	return uniqueGenres.size();
}

void searchBandByName(const string& bandName) {
	MusicBand band;
	ifstream bandFile("music_band.bin", ios::binary);

	if (!bandFile) {
		cout << "Error opening file!" << endl;
		return;
	}

	bool found = false;

	while (bandFile.read(reinterpret_cast<char*>(&band), sizeof(band))) {
		if (band.name == bandName) {
			found = true;
			cout << "Band found with the name '" << bandName << "':" << endl;

			bandInfoForm(band);
			cout << left << setw(30) << band.name << setw(20) << band.foundationDate << setw(30) << band.musicianName << setw(20) << band.genre << endl;
			cout << endl;
			break;
		}
	}

	if (!found) {
		cout << "No band with the name '" << bandName << "' found.\n\n";
	}

	bandFile.close();
}