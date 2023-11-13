#include <iostream>
#include <string>
#include <fstream>
#include <ctime>

using namespace std;

struct footballGames{
    int goals1{}, goals2{};
    string teamName1, teamName2;
};

int main() {
    // Olvassa be a fájlt egy olyan adatszerkezetbe, amit az alábbi kérdésekhez
    // fel tud használni és írassa ki hány mérkőzés volt a fordulóban!
    ifstream inputFile;
    inputFile.open("..//merkozes.txt"); // Ez VS community-ban ("merkozes.txt")

    if (!inputFile.is_open()) {
        cerr << "Failed to open the file." << endl;
        return 1;
    }
    footballGames games[8];

    int count = 0;
    for (int i = 0; i < 8 && !inputFile.eof(); i++) {
        count ++;
        inputFile >> games[i].goals1;
        inputFile >> games[i].goals2;
        inputFile >> games[i].teamName1;
        inputFile >> games[i].teamName2;
    }
    cout << "Merkozesek szama: " << count << endl;

    // Mennyi csapat nyert otthon?
    int homeWins = 0;
    for (const footballGames& game : games) // footballGames game : games
        if (game.goals1 > game.goals2)
            homeWins ++;
    cout << endl << homeWins << " csapat nyert otthon." << endl;

    // Melyik csapat rúgta a legtöbb gólt?
    int mostGoals = 0; string teamName;
    for (const footballGames& game : games) {
        if (game.goals1 > mostGoals) {
            mostGoals = game.goals1;
            teamName = game.teamName1;
        }
        if (game.goals2 > mostGoals) {
            mostGoals = game.goals2;
            teamName = game.teamName2;
        }
    }
    cout << "\nLegtobb golt rugo csapat: " << teamName << "\nGolok szama: " << mostGoals << endl;

    // Volt-e döntetlen?
    bool draw = false;
    for (const footballGames& game : games)
        if (game.goals1 == game.goals2)
            draw = true;
    if (draw)
        cout << "\nVolt dontetlen merkozes." << endl;
    else
        cout << "\nNem volt dontetlen merkozes." << endl;

    // Mennyi gólt lőtt az ute?
    int sum = 0;
    for (const footballGames& game : games) {
        if (game.teamName1 == "ute")
            sum += game.goals1;
        if (game.teamName2 == "ute")
            sum += game.goals2;
    }

    // Másik megoldás
//    const char teamLookingFor[] = "ute";
//    for (const footballGames& game : games) {
//        if (equal(game.teamName1.begin(), game.teamName1.end(), teamLookingFor))
//            sum += game.goals1;
//        if (equal(game.teamName2.begin(), game.teamName2.end(), teamLookingFor))
//            sum += game.goals2;
//    }
    cout << "\nUTE goljainak szama: " << sum << endl;

    return 0;
}
