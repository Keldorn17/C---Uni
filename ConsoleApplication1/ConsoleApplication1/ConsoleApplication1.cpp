#include <iostream>
#include <iomanip>

struct worker
{
    char name[25];
    int salary;
};

using namespace std;
int main() {
    worker group[3];
    int sum = 0;
    for (int i = 0; i < 3; i++)
    {
        cout << "What is the name of the " << i + 1 << ". employee: ";
        cin.getline(group[i].name, 25);
        cout << "The amount of salary: ";
        cin >> group[i].salary;
        cin.ignore();
    }
    cout.setf(ios::left);
    for (worker i : group)
    {
        cout << setw(25) << i.name << "\t" << i.salary << endl;
        sum += i.salary;
    }
    cout << "\nThe summary of the groups salary is $" << sum << endl;
    return 0;
}
