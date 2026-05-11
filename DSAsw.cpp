#include <iostream>   
#include <string>     
#include <vector>     
#include <iomanip>    
#include <sstream>    

using namespace std;  

struct ScrollSession {
    string appName;      
    string date;         
    string startTime;    
    string endTime;
    int    duration;     
    string contentType;  
    int    goalMinutes;  
};

vector<ScrollSession> sessionList;  

void displayMenu();
void logSession();
void computeTotals();
void generateFeedback();

void printLine(char c = '-', int length = 50);
void pauseScreen();
int  timeToMinutes(string timeStr);

// Validation helpers
bool isValidDate(string s) {
    if (s.length() != 10 || s[4] != '-' || s[7] != '-') return false;
    int y, m, d;
    char c1, c2;
    stringstream ss(s);
    ss >> y >> c1 >> m >> c2 >> d;
    return !ss.fail() && m >= 1 && m <= 12 && d >= 1 && d <= 31;
}

bool isValidTime(string s) {
    if (s.length() != 5 || s[2] != ':') return false;
    int h, m; char c;
    stringstream ss(s);
    ss >> h >> c >> m;
    return !ss.fail() && h >= 0 && h <= 23 && m >= 0 && m <= 59;
}

string getValidatedInput(string prompt, string errorMsg, bool (*isValid)(string)) {
    string input;
    cout << prompt;
    getline(cin, input);
    while (!isValid(input)) {
        cout << "  " << errorMsg;
        getline(cin, input);
    }
    return input;
}

int main() {

    int choice = -1;  

    cout << "\n";
    cout << "  ================================\n";
    cout << "   SCROLLWISE - Habit Monitor\n";
    cout << "  ================================\n";
    cout << "  Track your digital scrolling\n";
    cout << "  habits, one session at a time.\n";
    cout << "  ================================\n\n";

    while (choice != 0) {

        displayMenu();  

        cout << "  Enter your choice: ";
        cin  >> choice;
        cin.ignore();   

        if (choice == 1) {
            logSession();
        } else if (choice == 2) {
            computeTotals();
        } else if (choice == 3) {
            generateFeedback();
        } else if (choice == 0) {
            cout << "\n  Goodbye! Keep scrolling mindfully. :)\n\n";
        } else {
            cout << "\n  Invalid choice. Please enter 0 to 3.\n";
        }
    }

    return 0;  
}

void displayMenu() {
    cout << "\n";
    printLine('=');
    cout << "  MAIN MENU\n";
    printLine('=');
    cout << "  [1] Log a Scrolling Session\n";
    cout << "  [2] Compute Totals (per app / overall)\n";
    cout << "  [3] Generate Feedback / Insights\n";
    cout << "  [0] Exit\n";
    printLine('=');
}

void logSession() {

    ScrollSession newSession;  

    cout << "\n";
    printLine();
    cout << "  LOG A SCROLLING SESSION\n";
    printLine();

    cout << "  App Name (e.g. TikTok, Facebook): ";
    getline(cin, newSession.appName);

    // validated date and time inputs
    newSession.date      = getValidatedInput("  Date (e.g. 2026-03-01): ",  "Invalid date. Use YYYY-MM-DD: ",  isValidDate);
    newSession.startTime = getValidatedInput("  Start Time (e.g. 08:30): ", "Invalid time. Use HH:MM: ",       isValidTime);
    newSession.endTime   = getValidatedInput("  End Time   (e.g. 09:00): ", "Invalid time. Use HH:MM: ",       isValidTime);

    int startMins = timeToMinutes(newSession.startTime);
    int endMins   = timeToMinutes(newSession.endTime);

    while (endMins <= startMins) {
        cout << "  End time must be later than start time. Re-enter.\n";
        newSession.startTime = getValidatedInput("  Start Time (e.g. 08:30 or 13:00): ", "Invalid time. Use HH:MM (00-23): ", isValidTime);
        newSession.endTime   = getValidatedInput("  End Time   (e.g. 09:00 or 14:30): ", "Invalid time. Use HH:MM (00-23): ", isValidTime);
        startMins = timeToMinutes(newSession.startTime);
        endMins   = timeToMinutes(newSession.endTime);
    }
 
    newSession.duration = endMins - startMins;
    cout << "  Duration calculated: " << newSession.duration << " minute(s)\n";

    cout << "  Content Type (Entertainment / News / Education / Other): ";
    getline(cin, newSession.contentType);

    int  existingGoal = 0;
    bool goalFound    = false;

    for (size_t i = 0; i < sessionList.size(); i++) {
        if (sessionList[i].date == newSession.date) {
            existingGoal = sessionList[i].goalMinutes;
            goalFound    = true;
            break;  
        }
    }

    if (goalFound) {
        newSession.goalMinutes = existingGoal;
        cout << "  Goal for " << newSession.date << " already set: "
             << existingGoal << " minute(s). Using it.\n";
    } else {
        cout << "  No goal set for " << newSession.date << " yet.\n";
        cout << "  Set your scrolling goal for today (in minutes): ";
        cin  >> newSession.goalMinutes;
        cin.ignore();

        while (newSession.goalMinutes <= 0) {
            cout << "  Goal must be greater than 0. Try again: ";
            cin  >> newSession.goalMinutes;
            cin.ignore();
        }
    }

    sessionList.push_back(newSession);

    cout << "\n  Session saved successfully!\n";
    pauseScreen();
}

void computeTotals() {

    cout << "\n";
    printLine('=');
    cout << "  COMPUTE TOTALS\n";
    printLine('=');

    if (sessionList.empty()) {
        cout << "  No sessions to compute. Please log sessions first.\n";
        pauseScreen();
        return;  
    }

    int totalMinutes = 0;

    for (size_t i = 0; i < sessionList.size(); i++) {
        totalMinutes += sessionList[i].duration;
    }

    int hours   = totalMinutes / 60;
    int minutes = totalMinutes % 60;

    cout << "\n  Total Scrolling Time: "
         << hours << " hr(s) and " << minutes << " min(s)\n";
    cout << "  (" << totalMinutes << " minutes total)\n";

    vector<string> uniqueApps;

    for (size_t i = 0; i < sessionList.size(); i++) {
        string currentApp  = sessionList[i].appName;
        bool   alreadySeen = false;
        for (size_t j = 0; j < uniqueApps.size(); j++) {
            if (uniqueApps[j] == currentApp) {
                alreadySeen = true;
                break;  
            }
        }
        if (alreadySeen == false) {
            uniqueApps.push_back(currentApp);
        }
    }

    cout << "\n  Time per App:\n";
    printLine('-', 35);

    string mostUsedApp  = "";
    int    mostUsedMins = 0;

    for (size_t i = 0; i < uniqueApps.size(); i++) {
        int appTotal = 0;
        for (size_t j = 0; j < sessionList.size(); j++) {
            if (sessionList[j].appName == uniqueApps[i]) {
                appTotal += sessionList[j].duration;
            }
        }
        cout << "  " << left << setw(20) << uniqueApps[i]
             << ": " << appTotal << " mins\n";

        if (appTotal > mostUsedMins) {
            mostUsedMins = appTotal;
            mostUsedApp  = uniqueApps[i];
        }
    }

    printLine('-', 35);
    cout << "  Most Used App: " << mostUsedApp
         << " (" << mostUsedMins << " mins)\n";

    pauseScreen();
}

void generateFeedback() {

    cout << "\n";
    printLine('=');
    cout << "  GENERATE FEEDBACK / INSIGHTS\n";
    printLine('=');

    if (sessionList.empty()) {
        cout << "  No data yet. Please log sessions first.\n";
        pauseScreen();
        return;
    }

    int totalScrolled = 0;
    for (size_t i = 0; i < sessionList.size(); i++) {
        totalScrolled += sessionList[i].duration;
    }

    int  latestGoal = sessionList[sessionList.size() - 1].goalMinutes;
    bool hasGoal    = (latestGoal > 0);

    cout << "\n  *** ScrollWise Feedback ***\n\n";
    cout << "  Total time scrolled  : " << totalScrolled << " minutes\n";

    if (hasGoal) {
        cout << "  Your goal            : " << latestGoal << " minutes\n\n";

        if (totalScrolled <= latestGoal) {
            cout << "  Great job! You scrolled within your goal.\n";
            cout << "  Keep it up - mindful scrolling is a great habit!\n";
        } else if (totalScrolled <= latestGoal + 30) {
            cout << "  You slightly exceeded your goal by "
                 << (totalScrolled - latestGoal) << " minutes.\n";
            cout << "  Not bad! Try setting a timer next time to stay on track.\n";
        } else {
            cout << "  You exceeded your goal by "
                 << (totalScrolled - latestGoal) << " minutes.\n";
            cout << "  Consider reducing screen time tomorrow.\n";
            cout << "  Remember: small changes lead to big improvements!\n";
        }
    } else {
        cout << "\n  Tip: Set a daily goal when logging your first session\n";
        cout << "  so we can compare it against your actual usage!\n";
    }

    vector<string> uniqueContent;
    for (size_t i = 0; i < sessionList.size(); i++) {
        string ct   = sessionList[i].contentType;
        bool   seen = false;
        for (size_t j = 0; j < uniqueContent.size(); j++) {
            if (uniqueContent[j] == ct) { seen = true; break; }
        }
        if (seen == false) uniqueContent.push_back(ct);
    }

    string topContent     = "";
    int    topContentMins = 0;

    cout << "\n  ----- Content Breakdown -----\n";
    printLine('-', 35);

    for (size_t i = 0; i < uniqueContent.size(); i++) {
        int total = 0;
        for (size_t j = 0; j < sessionList.size(); j++) {
            if (sessionList[j].contentType == uniqueContent[i]) {
                total += sessionList[j].duration;
            }
        }
        cout << "  " << left << setw(18) << uniqueContent[i]
             << ": " << total << " mins\n";

        if (total > topContentMins) {
            topContentMins = total;
            topContent     = uniqueContent[i];
        }
    }

    printLine('-', 35);
    cout << "  Dominant Content: " << topContent
         << " (" << topContentMins << " mins)\n";

    if (topContent == "Entertainment") {
        cout << "  You spend most of your time on entertainment content.\n";
        cout << "  Try mixing in educational content for balance!\n";
    } else if (topContent == "News") {
        cout << "  You scroll a lot of news. Stay informed, but take breaks\n";
        cout << "  to avoid information overload.\n";
    } else if (topContent == "Education") {
        cout << "  Great! Most of your scrolling is educational.\n";
        cout << "  Keep using your screen time productively!\n";
    } else {
        cout << "  Consider being more intentional about what you scroll.\n";
    }

    cout << "\n  ----- General Insight -----\n";
    if (totalScrolled > 120) {
        cout << "  You've logged over 2 hours of scrolling.\n";
        cout << "  Consider taking breaks and doing offline activities.\n";
    } else if (totalScrolled > 60) {
        cout << "  You've logged about 1-2 hours of scrolling. Moderate use!\n";
        cout << "  Balance is key. Stay aware of your screen habits.\n";
    } else {
        cout << "  You've logged under 1 hour. That's great awareness!\n";
    }

    cout << "\n  Keep tracking. Awareness is the first step to change.\n";

    pauseScreen();
}

void printLine(char c, int length) {
    cout << "  ";
    for (int i = 0; i < length; i++) {
        cout << c;
    }
    cout << "\n";
}

int timeToMinutes(string timeStr) {
    int  hours = 0, minutes = 0;
    char colon;
    stringstream ss(timeStr);
    ss >> hours >> colon >> minutes;
    return hours * 60 + minutes;
}

void pauseScreen() {
    cout << "\n  Press Enter to continue...";
    cin.get();
}