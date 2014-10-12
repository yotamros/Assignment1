/*
 * Program: Elections
 * ------------------
 * The purpose of this program is to simulate the effect of error voting in
 * elections between two candidates.
 * The user is prompt to enter the number of voters, the percentage spread 
 * between candidates (example 0.005 for half a precent) and the voting error 
 * percentage (example 0.15 for 15%).  The program will output the chance of 
 * an invalid election result after 500 trials.
 */


#include <iostream>
#include <vector>
using namespace std;

const int TOTAL_PER = 100;
const int NUM_OF_TRIALS = 500;

struct dataT {
    int numOfVoters = -1;
    float preSpread = -1;
    float votingError = -1;
};

/*
 * Displays an error message and clears the input stream.
 */
void showError(string text) {
    cout << "Invalid entry, " << text << "." << endl;
    cin.clear();
    cin.ignore(numeric_limits <streamsize>::max(), '\n');
}

/*
 * Asks for input from the user about the number of voters, voting percentage 
 * error, and percentage spread between the candidates.  Validates the input.
 * @return data, dataT, a struct containing the 3 variables.
 */
dataT getUserInfo() {
    dataT data;
    while (data.numOfVoters == -1) {
        cout << "Enter the number of voters" << endl;
        cin >> data.numOfVoters;
        if (data.numOfVoters <= 0 || data.numOfVoters > 10000) {
            showError("enter a number between 1 and 10000");
            data.numOfVoters = -1;
        }
    }
    while (data.votingError == -1) {
        cout << "Enter the voting error percentage" << endl;
        cin >> data.votingError;
        if (data.votingError <= 0 || data.votingError >= 1) {
            showError("enter a number between 0 to 1");
            data.votingError = -1;
        }
    }
    while (data.preSpread == -1) {
        cout << "Enter the precentage spread between the two candidates." << endl;
        cin >> data.preSpread;
        if (data.preSpread <= 0 || data.preSpread >= 1) {
            showError("enter a number between 0 to 1");
            data.preSpread = -1;
        }
    }
    return data;
}

/*
 * First it calculates the results of a fair elections based on the user's input.
 * Then it calcuates the result for each candidate with x percent chance of the 
 * the vote being an error.  It runs the trail 500 times.  For each trial it 
 * compares the result of the fair vote and the error one.  If the error vote 
 * is different than the fair vote it adds a count to the invalidElection.
 * Finally it calcuates the total chance of wrong elections.
 * @return double, the chance of the elections being wrong.
 */
double RunElection(dataT data) {
    srand((unsigned int)time(NULL));
    int invalidElection = 0;
    int candACleanVote = data.numOfVoters/2 + (data.preSpread*data.numOfVoters)/2;
    int candBCleanVote = data.numOfVoters/2 - (data.preSpread*data.numOfVoters)/2;
    for (int i = 0; i < NUM_OF_TRIALS; i++) {
        int candASkewedVote = 0;
        int candBSkewedVote = 0;
        for (int i = 0; i < candACleanVote; i++) {
            int random = rand() % TOTAL_PER + 1;
            if (random < (TOTAL_PER - (data.votingError * TOTAL_PER)) ) {
                candASkewedVote++;
            } else {
                candBSkewedVote++;
            }
        }
        for (int i = 0; i < candBCleanVote; i++) {
            int random = rand() % TOTAL_PER + 1;
            if (random < (TOTAL_PER - (data.votingError * TOTAL_PER)) ) {
                candBSkewedVote++;
            } else {
                candASkewedVote++;
            }
        }
        if ((candACleanVote >= candBCleanVote && candBSkewedVote >= candASkewedVote)
            || (candACleanVote <= candBCleanVote && candBSkewedVote <= candASkewedVote)) {
            invalidElection++;
        }
    }
    return (double(TOTAL_PER)/NUM_OF_TRIALS) * double(invalidElection);
}

int main() {
    dataT data = getUserInfo();
    float invalidChance = RunElection(data);
    cout << "Chance of invalid elections after 500 trials = " << invalidChance << "%" << endl;
    return 0;
}



