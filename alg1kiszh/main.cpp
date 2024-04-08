#include <iostream>
#include <vector>
#include <random>
#include <string>
#include <fstream>
#include <array>
#include <iomanip>

int randInt(const int& min, const int& max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution dist(min, max);
    return dist(gen);
}

struct team {
    std::string name = {};
    int playerNumber = {};
    int height = {};
    int score = {};
};

void readFile(std::vector<team>& data, const std::string& filePath) {
    std::ifstream inputFile;
    inputFile.open(filePath);
    if (!inputFile.is_open()) {
        throw std::ios_base::failure("Failed to open file: " + filePath);
    }
    data.clear();
    team temp;
    while (inputFile >> temp.name >> temp.playerNumber) {
        temp.score = randInt(0, 35);
        temp.height = randInt(180, 225);
        data.push_back(temp);
    }
}

void setTeamNames(std::array<std::string, 2>& teamNames) {
    std::cout << "Kerem az elso csapat nevet: ";
    std::cin >> teamNames.at(0);
    std::string temp = teamNames.at(0);
    while (temp == teamNames.at(0)) {
        std::cout << "Kerem a masodik csapat nevet: ";
        std::cin >> temp;
    }
    teamNames.at(1) = temp;
}

int scoreSum(const std::vector<team>& data) {
    int sum = 0;
    for (auto const& list : data)
        sum += list.score;
    return sum;
}

void printData(const std::array<std::vector<team>, 2>& data, const std::array<std::string, 2>& nameData) {
    std::cout << std::left << std::setw(10) << nameData.at(0) << std::setw(15) << scoreSum(data.at(0));
    std::cout << std::left << std::setw(10) << nameData.at(1) << std::setw(15) << scoreSum(data.at(1));
    std::cout << std::endl;
    std::cout << std::left << std::setw(10) << "nev" << std::setw(15) << "pont";
    std::cout << std::left << std::setw(10) << "nev" << std::setw(15) << "pont";
    std::cout << std::endl;
    for (int i = 0; i < data.at(0).size(); i++) {
        std::cout << std::left << std::setw(10) << data.at(0).at(i).name << std::setw(15) << data.at(0).at(i).score;
        std::cout << std::left << std::setw(10) << data.at(1).at(i).name << std::setw(15) << data.at(1).at(i).score;
        std::cout << std::endl;
    }
}

void printDataHeight(const std::array<std::vector<team>, 2>& data, const std::array<std::string, 2>& nameData) {
    std::cout << std::left << std::setw(10) << nameData.at(0) << std::setw(25) << scoreSum(data.at(0));
    std::cout << std::left << std::setw(10) << nameData.at(1) << std::setw(25) << scoreSum(data.at(1));
    std::cout << std::endl;
    std::cout << std::left << std::setw(10) << "nev" << std::setw(15) << "pont" << std::setw(10) << "magassag";
    std::cout << std::left << std::setw(10) << "nev" << std::setw(15) << "pont" << std::setw(10) << "magassag";
    std::cout << std::endl;
    for (int i = 0; i < data.at(0).size(); i++) {
        std::cout << std::left << std::setw(10) << data.at(0).at(i).name << std::setw(15) << data.at(0).at(i).score << std::setw(10) << data.at(0).at(i).height;
        std::cout << std::left << std::setw(10) << data.at(1).at(i).name << std::setw(15) << data.at(1).at(i).score << std::setw(10) << data.at(1).at(i).height;
        std::cout << std::endl;
    }
}

std::string getSmallestPlayer(const std::vector<team>& data) {
    int index = 0;
    for (int i = 1; i < data.size(); i++)
        if (data.at(index).height > data.at(i).height)
            index = i;
    return data.at(index).name;
}
bool hadUselessTeammates(const std::array<std::vector<team>, 2>& data) {
    for (int i = 0; i < data.at(0).size(); i++)
        if (data.at(0).at(i).score == 0 || data.at(1).at(i).score == 0)
            return true;
    return false;
}

int bestScoreIndex(const std::vector<team>& data) {
    int index = 0;
    for (int i = 1; i < data.size(); i++)
        if (data.at(index).score < data.at(i).score)
            index = i;
    return index;
}

int worstScoreIndex(const std::vector<team>& data) {
    int index = 0;
    for (int i = 1; i < data.size(); i++)
        if (data.at(index).score > data.at(i).score)
            index = i;
    return index;
}

void bestScoreTeam(const std::array<std::vector<team>, 2>& data, const std::array<std::string, 2>& nameData) {
    int team1Best = bestScoreIndex(data.at(0));
    int team2Best = bestScoreIndex(data.at(1));
    if (data.at(0).at(team1Best).score > data.at(1).at(team2Best).score)
        std::cout << "Legtobb pontot szerzett csapat, jatekos, pontjai: " << nameData.at(0) << " "
                  << data.at(0).at(team1Best).name << " " << data.at(0).at(team1Best).score << std::endl;
    else
        std::cout << "Legtobb pontot szerzett csapat, jatekos, pontjai: " << nameData.at(1) << " "
                  << data.at(1).at(team2Best).name << " " << data.at(1).at(team2Best).score << std::endl;
}

int getExtremeIndex(const std::array<std::vector<team>, 2>& data) {
    int extreme1 = data.at(0).at(bestScoreIndex(data.at(0))).score - data.at(0).at(worstScoreIndex(data.at(0))).score;
    int extreme2 = data.at(1).at(bestScoreIndex(data.at(1))).score - data.at(1).at(worstScoreIndex(data.at(1))).score;
    if (extreme1 > extreme2)
        return 0;
    return 1;
}

void sortByHeight(std::vector<team>& data) {
    for (int i = 0; i < data.size() - 1; i++)
        for (int j = i + 1; j < data.size(); j++)
            if (data.at(i).height > data.at(j).height) {
                team temp = data.at(i);
                data.at(i) = data.at(j);
                data.at(j) = temp;
            }
}

int main() {
    std::array<std::vector<team>, 2> teams;
    std::array<std::string, 2> teamNames;
    readFile(teams.at(0), "../csapat1.txt"); // VS community: readFile(teams.at(0), "csapat1.txt")
    readFile(teams.at(1), "../csapat2.txt");
    setTeamNames(teamNames);
    printData(teams, teamNames);

    std::cout << "Legalacsonyabb jatekos a(z) " << teamNames.at(0) << "csapatban: " << getSmallestPlayer(teams.at(0)) << std::endl;

    if (hadUselessTeammates(teams))
        std::cout << "Volt olyan jatekos aki nem szerzett pontot." << std::endl;
    else
        std::cout << "Nem volt olyan jatekos aki nem szerzett pontot." << std::endl;

    bestScoreTeam(teams, teamNames);

    int team1ScoreSum = scoreSum(teams.at(0));
    int team2ScoreSum = scoreSum(teams.at(1));
    std::cout << teamNames.at(0) << " atlag pontja: " << team1ScoreSum / (double) teams.at(0).size() << std::endl;
    std::cout << teamNames.at(1) << " atlag pontja: " << team2ScoreSum / (double) teams.at(1).size() << std::endl;
    std::cout << "ketto csapat atlag pontja: " << (team1ScoreSum + team2ScoreSum) / (double) teams.at(1).size() << std::endl;

    std::cout << "Szelsoseget dobott csapat neve: " << teamNames.at(getExtremeIndex(teams)) << std::endl;

    sortByHeight(teams.at(0));
    sortByHeight(teams.at(1));
    printDataHeight(teams, teamNames);

    return 0;
}
