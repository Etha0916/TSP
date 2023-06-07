#include <bits/stdc++.h>

using namespace std;

class QM {
public:
    int numVariables;
    string dontCares;

    QM(int variables) : numVariables(variables) {
        dontCares.append(variables, '-');
    }

    vector<string> getVariables() {
        vector<string> variables;
        string letters[] = { "a", "b", "c", "d", "e", "f", "g", "h" };
        for (int i = 0; i < numVariables; i++)
            variables.push_back(letters[i]);

        return variables;
    }

    string convertToBinary(int decimal) {
        return bitset<8>(decimal).to_string().substr(8 - numVariables);
    }

    string padZeros(string binary) {
        int padding = numVariables - binary.length();
        return string(padding, '0') + binary;
    }

    bool isGreyCode(const string& term1, const string& term2) {
        int count = 0;
        for (int i = 0; i < term1.length(); i++) {
            if (term1[i] != term2[i])
                count++;
        }
        return (count == 1);
    }

    string replaceComplements(const string& term1, const string& term2) {
        string result = "";
        for (int i = 0; i < term1.length(); i++) {
            if (term1[i] != term2[i])
                result += '-';
            else
                result += term1[i];
        }
        return result;
    }

    bool isTermInVector(const vector<string>& vec, const string& term) {
        return find(vec.begin(), vec.end(), term) != vec.end();
    }

    vector<string> reduceMinterms(const vector<string>& minterms) {
        vector<string> reducedMinterms;
        int count = minterms.size();
        vector<int> checked(count, 0);

        for (int i = 0; i < count; i++) {
            for (int j = i; j < count; j++) {
                if (isGreyCode(minterms[i], minterms[j])) {
                    checked[i] = checked[j] = 1;
                    string complement = replaceComplements(minterms[i], minterms[j]);
                    if (!isTermInVector(reducedMinterms, complement))
                        reducedMinterms.push_back(complement);
                }
            }
        }

        for (int i = 0; i < count; i++) {
            if (checked[i] != 1 && !isTermInVector(reducedMinterms, minterms[i]))
                reducedMinterms.push_back(minterms[i]);
        }

        return reducedMinterms;
    }

    string getBooleanExpression(const string& term) {
        string expression = "";
        vector<string> variables = getVariables();
        if (term == dontCares)
            return "1";

        for (int i = 0; i < term.length(); i++) {
            if (term[i] != '-') {
                if (term[i] == '0')
                    expression += variables[i] + "\'";
                else
                    expression += variables[i];
            }
        }
        return expression;
    }

    bool areVectorsEqual(const vector<string>& vec1, const vector<string>& vec2) {
        if (vec1.size() != vec2.size())
            return false;

        vector<string> sortedVec1 = vec1;
        vector<string> sortedVec2 = vec2;
        sort(sortedVec1.begin(), sortedVec1.end());
        sort(sortedVec2.begin(), sortedVec2.end());

        for (int i = 0; i < vec1.size(); i++) {
            if (sortedVec1[i] != sortedVec2[i])
                return false;
        }
        return true;
    }
};

int main() {
    char choice = 'y';
    while (choice == 'y') {
        int numVars;
        cout << endl << "Enter the number of variables:" << endl;
        cin >> numVars;
        if (numVars > 8 || numVars < 1) {
            cout << "Invalid number of variables (1-8)" << endl;
            continue;
        }
        QM qm(numVars);

        string input;
        cout << "Enter the minterms (RANGE = 0-" << (1 << numVars) - 1 << ") separated by comma:" << endl;
        cin >> input;

        vector<string> minterms;
        size_t pos = 0;
        string token;
        while ((pos = input.find(',')) != string::npos) {
            token = input.substr(0, pos);
            int value = stoi(token);
            minterms.push_back(qm.padZeros(qm.convertToBinary(value)));
            input.erase(0, pos + 1);
        }
        minterms.push_back(qm.padZeros(qm.convertToBinary(stoi(input))));

        sort(minterms.begin(), minterms.end());

        do {
            minterms = qm.reduceMinterms(minterms);
            sort(minterms.begin(), minterms.end());
        } while (!qm.areVectorsEqual(minterms, qm.reduceMinterms(minterms)));

        cout << "The simplified expression in Sum-of-Products (SOP) format is as follows:" << endl;
        for (int i = 0; i < minterms.size() - 1; i++)
            cout << qm.getBooleanExpression(minterms[i]) << "+";
        cout << qm.getBooleanExpression(minterms.back()) << endl;


        cin >> choice;
    }

cout<<"created by Ethan";
    return 0;
}


