
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream> 
#include <iomanip>
#include <algorithm>
using namespace std;

void sum_spend_one(string* People, vector<vector<string>>& spendings, float* sum_one, int N, int size) {
	for (int name = 0; name < N; name++) {
		float sum = 0.0;
		for (int line = 0; line < size; line++) {
			if (spendings[line][0] == People[name]) {
				sum += stof(spendings[line][1]);
			}
		}
		sum_one[name] = sum;
	}
	
}

void had_to_pay(string* People, vector<vector<string>>& spendings, float* had_pay, int N, int size) {
    for (int name = 0; name < N; name++) {
        float sum = 0.0;
        string without = People[name];
        for (int line = 0; line < size; line++) {
            auto flag = find(spendings[line].begin()+2, spendings[line].end(), without);
            if (flag != spendings[line].end()) sum += 0.0;
            else {
                int kol_without = spendings[line].size() - 2;
                sum = sum + (stof(spendings[line][1])) / (N - kol_without);
            }
            
        }
        had_pay[name] = sum;   
    }
}

void substraction(float* sum_one, float* had_pay, int N, float* sub) {
    for (int name = 0; name < N; name++) {
        sub[name] = sum_one[name] - had_pay[name];
    }
}


int main()
{
	string f = "data_2.txt"; 
	ifstream file(f);
	if (!file.is_open()) {
		cerr << "Failed to open file: " << f << endl;
		return 1;
	}
	int N;
	file >> N;
	string* People = new string[N];
	for (int i = 0; i < N; i++) {
		file >> People[i];
	}
	vector<vector<string>> spendings; 
	string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        size_t slash = line.find('/');
        vector<string> data_arr;

        if (slash != string::npos) {
            size_t probel1 = line.find(' ');
            string payer = line.substr(0, probel1);
            string money = line.substr(probel1 + 1, slash - probel1 - 2);
            string recipients_str = line.substr(slash + 2);

            data_arr.push_back(payer);
            data_arr.push_back(money);

            
            stringstream s(recipients_str);
            string after_slash;
            while (getline(s, after_slash, ',')) {
                size_t start = after_slash.find_first_not_of(" \t");
                size_t end = after_slash.find_last_not_of(" \t");
                if (start != string::npos) {
                    data_arr.push_back(after_slash.substr(start, end - start + 1));
                }
            }
        }
        else {
            
            size_t probel1 = line.find(' ');
            string payer = line.substr(0, probel1);
            string money = line.substr(probel1 + 1);

            data_arr.push_back(payer);
            data_arr.push_back(money);
        }

        spendings.push_back(data_arr);
    }

	float* sum_one = new float[N]; 
    int size = spendings.size();
    sum_spend_one(People, spendings, sum_one, N, size);
    float* had_pay = new float[N]; 
    had_to_pay(People, spendings, had_pay, N, size);
    float* sub = new float[N]; 
    substraction(sum_one, had_pay, N, sub);
    cout << fixed << setprecision(1);
    

    for (int i = 0; i < N; i++) {
        cout << People[i] << " "<< sum_one[i] << " " << had_pay[i] << endl;
    }
    float* balance = new float[N];
    for (int i = 0; i < N; i++) {
        balance[i] = sub[i];
    }

    cout << endl;

    for (int i = 0; i < N; i++) {
        if (balance[i] < 0) {
            for (int j = 0; j < N; j++) {
                if (balance[j] > 0) {
                    float transfer = min(-balance[i], balance[j]);
                    cout << People[i] << " " << transfer << " " << People[j] << endl;
                    balance[i] += transfer;
                    balance[j] -= transfer;
                    if (balance[i] >= 0) break;
                }
            }
        }
    }

    delete[] People;
    delete[] sum_one;
    delete[] had_pay;
    delete[] sub;
    delete[] balance;

	file.close();
	return 0;
}

