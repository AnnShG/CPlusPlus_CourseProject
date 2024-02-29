#pragma once
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <iomanip>

using namespace std;

enum menu {
	Add = 1,
	Show,
	Remove,
	Search,
	Filter,
	Sort,
	Calculate,
	Exit
};

struct MusicBand {
	char name[30];
	int foundationDate;
	char musicianName[50];
	char genre[20];
};

void addBand(const MusicBand& b);
void removeBandFile();
void bandInfoForm(const MusicBand& band);
void filter();
void sortByName();
void sortByDate();
int countRecords();
int countUniqueGenres();
void searchBandByName(const string& bandName);