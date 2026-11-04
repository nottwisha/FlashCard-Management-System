#include<iostream>
#include<vector>
#include<string>
#include<fstream>
using namespace std;

class Flashcard {
public:
    int masteryscore;
    int wrongcount;
    string question;
    string answer;
    string choice1;
    string category;
    string prerequisite;

    Flashcard(string q, string a, string c) {
        question = q;
        answer = a;
        category = c;
        masteryscore = 0;
        wrongcount = 0;
        prerequisite = "none";
    }

    virtual ~Flashcard() {}

    virtual void display() {
        cout << "Category: " << category << endl;
        cout << question << endl;
    }

    bool check(string a) {
        if (answer == a) {
            if (masteryscore <= 90) {
                masteryscore = masteryscore + 10;
            }
            else {
                masteryscore = 100;
            }
            cout << "Correct Answer!!!" << endl;
            return true;
        }
        else {
            if (masteryscore > 0) {
                masteryscore = masteryscore - 10;
            }
            else {
                masteryscore = 0;
            }
            wrongcount++;
            cout << "Wrong Answer!" << endl;
            return false;
        }
    }

    int priority() {
        return masteryscore - wrongcount * 5;
    }

    void displaymastery() {
        cout << "Current Mastery Score: " << masteryscore << endl << endl;
    }
};

class VocabularyCard : public Flashcard {
public:
    VocabularyCard(string q, string a, string c) : Flashcard(q, a, c) {}
    void display() override {
        cout << "Category : Vocabulary" << endl;
        cout << question << endl;
    }
};

class GrammarCard : public Flashcard {
public:
    GrammarCard(string q, string a, string c) : Flashcard(q, a, c) {}
    void display() override {
        cout << "Category : Grammar" << endl;
        cout << question << endl;
    }
};

class FormulaCard : public Flashcard {
public:
    FormulaCard(string q, string a, string c) : Flashcard(q, a, c) {}
    void display() override {
        cout << "Category : Formula" << endl;
        cout << question << endl;
    }
};

class MathCard : public Flashcard {
public:
    MathCard(string q, string a, string c) : Flashcard(q, a, c) {}
    void display() override {
        cout << "Category : Math" << endl;
        cout << question << endl;
    }
};

Flashcard* makeCard(string cat, string ques, string ans) {
    if (cat == "Math") {
        return new MathCard(ques, ans, cat);
    }
    else if (cat == "Vocabulary") {
        return new VocabularyCard(ques, ans, cat);
    }
    else if (cat == "Grammar") {
        return new GrammarCard(ques, ans, cat);
    }
    else {
        return new FormulaCard(ques, ans, cat);
    }
}

void saveCards(vector<Flashcard*>& cards) {
    ofstream file("cards.txt");
    if (!file) {
        cout << "Could not save cards." << endl;
        return;
    }

    for (int i = 0; i < cards.size(); i++) {
        file << cards[i]->category << endl;
        file << cards[i]->question << endl;
        file << cards[i]->answer << endl;
        file << cards[i]->masteryscore << endl;
        file << cards[i]->wrongcount << endl;
        file << cards[i]->prerequisite << endl;
    }
    file.close();
}

void loadCards(vector<Flashcard*>& cards) {
    ifstream file("cards.txt");
    if (!file) {
        return;
    }

    string cat, ques, ans, prereq;
    int score, wrong;

    while (getline(file, cat)) {
        if (!getline(file, ques) || !getline(file, ans)) {
            break;
        }
        if (!(file >> score) || !(file >> wrong)) {
            break;
        }
        file.ignore(10000, '\n');
        if (!getline(file, prereq)) {
            break;
        }

        Flashcard* card = makeCard(cat, ques, ans);
        card->masteryscore = score;
        card->wrongcount = wrong;
        card->prerequisite = prereq;
        cards.push_back(card);
    }
    file.close();
}

void addCard(vector<Flashcard*>& cards) {
    string cat, ques, ans, prereq;

    cout << "Category (Math / Vocabulary / Grammar / Formula): ";
    getline(cin, cat);

    if (cat != "Math" && cat != "Vocabulary" &&
        cat != "Grammar" && cat != "Formula") {
        cout << "Invalid category." << endl;
        return;
    }

    cout << "Question: ";
    getline(cin, ques);
    cout << "Answer: ";
    getline(cin, ans);
    cout << "Prerequisite question (type none if there is no prerequisite): ";
    getline(cin, prereq);

    Flashcard* card = makeCard(cat, ques, ans);
    card->prerequisite = prereq;
    cards.push_back(card);
    saveCards(cards);
    cout << "Card added!" << endl;
}

bool isUnlocked(Flashcard* card, vector<Flashcard*>& cards) {
    if (card->prerequisite == "none") {
        return true;
    }

    for (int i = 0; i < cards.size(); i++) {
        if (cards[i]->question == card->prerequisite) {
            return cards[i]->masteryscore >= 30;
        }
    }
    return false;
}

int main() {
    int lowestindex;
    string a;
    string choice = "yes", categorychoice;
    int choice2, menuChoice;
    vector<Flashcard*> flashcard;

    loadCards(flashcard);

    if (flashcard.size() == 0) {
        flashcard.push_back(new MathCard("What is 10+2?", "12", "Math"));
        flashcard.push_back(new VocabularyCard("What is french for love?", "amour", "Vocabulary"));
        flashcard.push_back(new VocabularyCard("French for hello?", "bonjour", "Vocabulary"));
        flashcard.push_back(new GrammarCard("What is a noun?", "A naming word", "Grammar"));
        flashcard.push_back(new FormulaCard("What is the formula for the area of a circle?", "pi*r*r", "Formula"));
        flashcard[2]->prerequisite = "What is french for love?";
    }

    do {
        cout << "\n";
        cout << "*********************************************\n";
        cout << "                STUDYBUDDY                    \n";
        cout << "*********************************************\n";
        cout << "       ADAPTIVE SPACED-REPETITION SYSTEM       \n";
        cout << "--------------------------------------------\n";
        cout << "                 MAIN MENU                   \n";
        cout << "--------------------------------------------\n";
        cout << "  1. Start Studying\n";
        cout << "  2. View Learning Progress\n";
        cout << "  3. View Flashcard Categories\n";
        cout << "  4. Add a New Flashcard\n";
        cout << "  5. Exit\n";
        cout << "--------------------------------------------\n";
        cout << "  Enter your choice: ";

        if (!(cin >> menuChoice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input. Enter a number from 1-5." << endl;
            continue;
        }
        cin.ignore(10000, '\n');

        switch (menuChoice) {
        case 1: {
            cout << "\nStarting Study Session\n";
            cout << "Please Select a Category:\n";
            cout << "1. Math\n2. Vocabulary\n3. Grammar\n4. Formula\n5. All\n";
            cin >> choice2;
            cin.ignore(10000, '\n');

            switch (choice2) {
            case 1: categorychoice = "Math"; break;
            case 2: categorychoice = "Vocabulary"; break;
            case 3: categorychoice = "Grammar"; break;
            case 4: categorychoice = "Formula"; break;
            case 5: categorychoice = "All"; break;
            default:
                cout << "Invalid Choice." << endl;
                break;
            }

            if (choice2 < 1 || choice2 > 5) {
                break;
            }

            do {
                vector<bool> asked(flashcard.size(), false);
                int total = 0;

                for (int i = 0; i < flashcard.size(); i++) {
                    if (categorychoice == "All" || flashcard[i]->category == categorychoice) {
                        total++;
                    }
                }

                for (int j = 0; j < total; j++) {
                    lowestindex = -1;

                    for (int i = 0; i < flashcard.size(); i++) {
                        bool rightcategory =
                            flashcard[i]->category == categorychoice ||
                            categorychoice == "All";

                        if (!asked[i] && rightcategory && isUnlocked(flashcard[i], flashcard)) {
                            if (lowestindex == -1 ||
                                flashcard[i]->priority() < flashcard[lowestindex]->priority()) {
                                lowestindex = i;
                            }
                        }
                    }

                    if (lowestindex == -1) {
                        cout << "No more unlocked cards right now." << endl;
                        break;
                    }

                    asked[lowestindex] = true;
                    flashcard[lowestindex]->display();

                    cout << "Your answer (or type quit): ";
                    getline(cin, a);

                    if (a == "quit") {
                        break;
                    }

                    bool correct = flashcard[lowestindex]->check(a);

                    if (!correct) {
                        cout << "Do you want to reveal the answer? (yes/no): ";
                        cin >> choice;
                        cin.ignore(10000, '\n');

                        if (choice == "yes") {
                            cout << "The correct answer is: "
                                << flashcard[lowestindex]->answer << endl;
                        }
                    }

                    flashcard[lowestindex]->displaymastery();
                    saveCards(flashcard);
                }

                cout << "Do you want to study again? (yes/no): ";
                getline(cin, choice);
            } while (choice == "yes");

            break;
        }

        case 2: {
            cout << "\n========== LEARNING PROGRESS ==========\n";
            if (flashcard.size() == 0) {
                cout << "No flashcards available." << endl;
                break;
            }

            int totalMastery = 0;
            for (int i = 0; i < flashcard.size(); i++) {
                cout << "\nCategory: " << flashcard[i]->category << endl;
                cout << "Question: " << flashcard[i]->question << endl;
                flashcard[i]->displaymastery();
                cout << "Wrong attempts: " << flashcard[i]->wrongcount << endl;
                totalMastery += flashcard[i]->masteryscore;
            }

            cout << "Overall Average Mastery: "
                << static_cast<double>(totalMastery) / flashcard.size()
                << "%" << endl;
            break;
        }

        case 3: {
            cout << "\n========== FLASHCARD CATEGORIES ==========\n";
            string categories[] = { "Math", "Vocabulary", "Grammar", "Formula" };

            for (int c = 0; c < 4; c++) {
                int count = 0;
                for (int i = 0; i < flashcard.size(); i++) {
                    if (flashcard[i]->category == categories[c]) {
                        count++;
                    }
                }
                cout << categories[c] << ": " << count << " card(s)" << endl;
            }
            cout << "Total flashcards: " << flashcard.size() << endl;
            break;
        }

        case 4:
            addCard(flashcard);
            break;

        case 5:
            saveCards(flashcard);
            cout << "\nThank you for using StudyBuddy!\n";
            cout << "Keep learning, keep growing!\n";
            break;

        default:
            cout << "\nInvalid choice. Please select 1-5.\n";
        }

    } while (menuChoice != 5);

    saveCards(flashcard);

    for (int i = 0; i < flashcard.size(); i++) {
        delete flashcard[i];
    }

    return 0;
}