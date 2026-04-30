#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <limits> 
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#define BOLD    "\033[1m"
using namespace std;
const string USER_FILE = "users.txt";
const string DOCTOR_FILE = "doctors.txt";
const string APPOINTMENT_FILE = "appointments.txt";
const string STAFF_ID = "staff101"; 
const string STAFF_PASS = "admin123";
struct User {
    string name;
    string cnic;   
    string contact;  
    char gender;     
    string password;
};
struct Doctor {
    int id;
    string name;
    string qualification;
    double fee;
    string timeSlots[5];
};
struct Appointment {
    int token;
    string userCnic;
    int doctorId;
    string timeSlot;
    string status;
};
vector<User> users;
vector<Doctor> doctors;
vector<Appointment> appointments;
void displayMainMenu();
void userPortalMenu();
void staffPortalMenu();
void loadData();
void saveUsers();
void saveAppointments();
void populateDefaultDoctors();
void loadDoctors();
void signup();
bool login(string& loggedInCnic);
void userOptions(const string& cnic);
void addAppointment(const string& userCnic);
void viewAppointments(const string& userCnic);
void cancelAppointment(const string& userCnic);
void staffOptions();
void viewAllAppointments();
void updateAppointmentStatus();
void alterDoctorInfo();
void displayAllUsers();
bool isCnicUnique(const string& cnic);
bool isCnicValid(const string& cnic);
bool isContactValid(const string& contact);
bool isGenderValid(char gender);
void clearInputBuffer();
void displayDoctorTable();
int generateToken();
void displayAppointmentTable(const vector<Appointment>& apts);
bool confirmActionRecursive(const string& prompt) {
    char choice;
    cout << prompt << " (y/n): ";
    cin >> choice;
    clearInputBuffer();
    choice = toupper(choice);

    if (choice == 'Y') return true;
    if (choice == 'N') return false;

    cout <<RED<< "[ALERT] Invalid input. Please enter 'y' or 'n'." <<RESET<< endl;
    return confirmActionRecursive(prompt);
}
void alert(string msg, bool success = true) {
    if (success)
        cout << GREEN << "[SUCCESS] " << msg << RESET << endl;
    else
        cout << RED << "[ERROR] " << msg << RESET << endl;
}

int main() {
    loadData();
    displayMainMenu();
    return 0;
}
void displayMainMenu() {
    int choice;
    do {
        cout <<setw(12)<<BLUE<< "WELCOME TO CLINIC MANAGEMENT SYSTEM" <<RESET<< endl;
        cout << " " << endl;
        cout << "1. User Portal" << endl;
        cout << "2. Staff Portal" << endl;
        cout << "3. Display Doctors & Fees" << endl;
        cout << "4. Exit" << endl;
        cout << " " << endl;
        cout << "Enter your choice: ";

        if (!(cin >> choice)) {
            alert("Invalid input! Please enter a number", false);
            clearInputBuffer();
            choice = 0; 
            continue;
        }
        clearInputBuffer();
        switch (choice) {
        case 1: userPortalMenu(); break;
        case 2: staffPortalMenu(); break;
        case 3: displayDoctorTable(); break;
        case 4: cout <<YELLOW<< "GOODBYE Exiting the application. Thank you!" <<RESET<< endl; break;
        default: alert("Invalid choice. Please try again.", false); break;
        }
    } while (choice != 4);
}
void userPortalMenu() {
    int choice;
    string loggedInCnic = "";

    do {
        cout <<CYAN<<setw(12)<< "USER PORTAL" <<RESET<< endl;
        cout << " " << endl;
        cout << "1. Sign Up" << endl;
        cout << "2. Log In" << endl;
        cout << "3. Back to Main Menu" << endl;
        cout << " " << endl;
        cout << "Enter your choice: ";
        if (!(cin >> choice)) {
            clearInputBuffer();
            choice = 0;
            continue;
        }
        clearInputBuffer();
        switch (choice) {
        case 1: signup(); break;
        case 2:
            if (login(loggedInCnic)) {
                userOptions(loggedInCnic);
                choice = 3;
            }
            break;
        case 3: cout <<YELLOW<< "INFO Returning to Main Menu." <<RESET<< endl; break;
        default: alert(" Invalid choice. Please try again.", false); break;
        }
    } while (choice != 3);
}
void signup() {
    User newUser;
    cout <<CYAN<<setw(12)<< "\nSIGN UP" <<RESET<< endl;
    cout << "Enter Name: ";
    getline(cin, newUser.name);
    do {
        cout << "Enter CNIC (13 digits): ";
        getline(cin, newUser.cnic);
        if (!isCnicValid(newUser.cnic)) {
            alert("CNIC must be exactly 13 digits and numeric.", false);
        }
        else if (!isCnicUnique(newUser.cnic)) {
            alert("CNIC already registered.Please login or check your number.", false);
        }
    } while (!isCnicValid(newUser.cnic) || !isCnicUnique(newUser.cnic));

    do {
        cout << "Enter Contact No (11 digits): ";
        getline(cin, newUser.contact);
        if (!isContactValid(newUser.contact)) {
            alert("Contact must be exactly 11 digits and numeric.", false);
        }
    } while (!isContactValid(newUser.contact));
    char genderInput;
    do {
        cout << "Enter Gender (M/F/X): ";
        cin >> genderInput;
        clearInputBuffer();
        newUser.gender = toupper(genderInput);
        if (!isGenderValid(newUser.gender)) {
            alert("Invalid gender. Must be 'M', 'F', or 'X'.", false);
        }
    } while (!isGenderValid(newUser.gender));
    cout << "Enter Password (must be unique for you): ";
    getline(cin, newUser.password);
    users.push_back(newUser);
    saveUsers();
    cout << GREEN<<"\nSignup successful! Welcome, " << newUser.name << "!" <<RESET<< endl;
}
bool login(string& loggedInCnic) {
    string cnic, password;
    cout <<CYAN<< "\n LOG IN " <<RESET<< endl;
    do {
        cout << "Enter CNIC (13 digits): ";
        getline(cin, cnic);
        if (!isCnicValid(cnic)) {
            alert("Invalid CNIC format.Enter a valid 13 - digit number.", false);
        }
    } while (!isCnicValid(cnic));
    cout << "Enter Password: ";
    getline(cin, password);
    for (size_t i = 0; i < users.size(); ++i) {
        if (users[i].cnic == cnic) {
            if (users[i].password == password) {
                loggedInCnic = cnic;
                alert("Successfully logged in as ", true);
                    cout<< GREEN<<users[i].name << "." <<RESET <<endl;
                displayDoctorTable();
                return true;
            }
            else {
                alert("\nWrong password for CNIC: ", false);
                    cout<<GREEN<<cnic << "." << RESET<<endl;
                return false;
            }
        }
    }
    alert("CNIC not found. Please sign up first.", false);
    return false;
}
void userOptions(const string& cnic) {
    int choice;
    do {
        cout <<CYAN<< "USER OPTIONS: CNIC " << cnic << RESET<<endl;
        cout << " " << endl;
        cout << "1. Add Appointment" << endl;
        cout << "2. View My Appointments" << endl;
        cout << "3. Cancel Appointment (by Token ID)" << endl;
        cout << "4. Log Out" << endl;
        cout << " " << endl;
        cout << "Enter your choice: ";
        if (!(cin >> choice)) {
            clearInputBuffer();
            choice = 0;
            continue;
        }
        clearInputBuffer();
        switch (choice) {
        case 1: addAppointment(cnic); break;
        case 2: viewAppointments(cnic); break;
        case 3: cancelAppointment(cnic); break;
        case 4: cout <<YELLOW<< "[INFO] Logging out." <<RESET<< endl; break;
        default: cout <<RED<<" "<<RESET << endl; break;
        }
    } while (choice != 4);
}
void addAppointment(const string& userCnic) {
    if (doctors.empty()) {
        cout <<YELLOW<< "[ALERT] No doctors available at the moment. Please try again later." <<RESET<< endl;
        return;
    }

    displayDoctorTable();

    int docId, slotIndex;
    cout <<YELLOW<< "\nBOOK APPOINTMENT" <<RESET<< endl;
    size_t docIndex = -1;
    do {
        cout << "Enter Doctor ID to book: ";
        if (!(cin >> docId)) {
            clearInputBuffer();
            docId = -1;
            cout << RED<<"[ALERT] Invalid input." <<RESET<< endl;
            continue;
        }
        for (size_t i = 0; i < doctors.size(); ++i) {
            if (doctors[i].id == docId) {
                docIndex = i;
                break;
            }
        }
        if (docIndex == -1) {
            cout <<RED<< "[ALERT] Doctor ID not found. Please select from the list." <<RESET<< endl;
        }
    } while (docIndex == -1);
    bool slotAvailable = false;
    do {
        cout << "Available time slots (Enter 1-5):" << endl;
        for (int i = 0; i < 5; ++i) {
            cout << (i + 1) << ". " << doctors[docIndex].timeSlots[i];
            bool booked = false;
            for (const auto& apt : appointments) {
                if (apt.doctorId == docId && apt.timeSlot == doctors[docIndex].timeSlots[i] && apt.status == "Pending") {
                    booked = true;
                    break;
                }
            }
            if (booked) {
                cout << YELLOW<<" BOOKED"<<RESET;
            }
            cout << endl;
        }

        cout << "Enter slot number (1-5): ";
        if (!(cin >> slotIndex) || slotIndex < 1 || slotIndex > 5) {
            cout << RED<<"[ALERT] Invalid slot number." <<RESET<< endl;
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();
        slotAvailable = true;
        string selectedTimeSlot = doctors[docIndex].timeSlots[slotIndex - 1];
        for (const auto& apt : appointments) {
            if (apt.doctorId == docId && apt.timeSlot == selectedTimeSlot && apt.status == "Pending") {
                cout << RED<<"[ALERT] This time slot is already booked. Please choose another." <<RESET<< endl;
                slotAvailable = false;
                break;
            }
        }
    } while (!slotAvailable);
    Appointment newApt;
    newApt.token = generateToken();
    newApt.userCnic = userCnic;
    newApt.doctorId = docId;
    newApt.timeSlot = doctors[docIndex].timeSlots[slotIndex - 1];
    newApt.status = "Pending";

    appointments.push_back(newApt);
    saveAppointments();

    cout << GREEN<<"\[SUCCESS] Appointment booked!" << RESET<<endl;
    cout << "Your Token ID: " << newApt.token << endl;
    cout << "Doctor: " << doctors[docIndex].name << ", Time: " << newApt.timeSlot << endl;
}
void viewAppointments(const string& userCnic) {
    cout <<CYAN<<setw(12)<< "\nMY APPOINTMENTS" <<RESET<< endl;
    vector<Appointment> userApts;
    for (const auto& apt : appointments) {
        if (apt.userCnic == userCnic) {
            userApts.push_back(apt);
        }
    }
    if (userApts.empty()) {
        cout << YELLOW<<"[INFO] You have no booked appointments." <<RESET<< endl;
        return;
    }
    displayAppointmentTable(userApts);
    if (confirmActionRecursive("Do you want to view the status of a specific appointment?")) {
        int tokenId;
        cout << "Enter Token ID: ";
        if (!(cin >> tokenId)) {
            clearInputBuffer();
            cout <<RED<< "[ALERT] Invalid input." <<RESET<< endl;
            return;
        }
        clearInputBuffer();
        bool found = false;
        for (const auto& apt : userApts) {
            if (apt.token == tokenId) {
                cout << "\nStatus for Token " << apt.token << ":   " << apt.status << "*" << endl;
                found = true;
                break;
            }
        }
        if (!found) {
            cout << RED<<"[ALERT] Token ID not found in your appointments." <<RESET<< endl;
        }
    }
}
void cancelAppointment(const string& userCnic) {
    cout <<CYAN<<setw(12)<< "\nCANCEL APPOINTMENT" <<RESET<< endl;
    int tokenId;
    cout << "Enter Token ID of the appointment to cancel: ";
    if (!(cin >> tokenId)) {
        clearInputBuffer();
        cout <<RED<< "[ALERT] Invalid input." <<RESET<< endl;
        return;
    }
    clearInputBuffer();
    for (size_t i = 0; i < appointments.size(); ++i) {
        if (appointments[i].token == tokenId && appointments[i].userCnic == userCnic) {
            if (confirmActionRecursive("Are you sure you want to cancel appointment with Token " + to_string(tokenId) + "?")) {
                appointments.erase(appointments.begin() + i);
                saveAppointments(); 
                cout <<GREEN<< "\n[SUCCESS] Appointment with Token " << tokenId << " has been cancelled." <<RESET<< endl;
                return;
            }
            else {
                cout <<YELLOW<< "[INFO] Cancellation aborted." <<RESET<< endl;
                return;
            }
        }
    }

    cout <<RED<< "[ALERT] Appointment with Token " << tokenId << " not found or does not belong to your account." <<RESET<< endl;
}
void staffPortalMenu() {
    string id, pass;
    cout <<CYAN<<setw(12)<< "\nSTAFF LOGIN " <<RESET<< endl;
    cout << "Enter Staff ID: ";
    getline(cin, id);
    cout << "Enter Password: ";
    getline(cin, pass);

    if (id == STAFF_ID && pass == STAFF_PASS) {
        cout << GREEN<<"\n[SUCCESS] Staff member successfully logged in!" <<RESET<< endl;
        staffOptions();
    }
    else {
        cout <<RED<< "\n[ALERT] Invalid Staff ID or Password." <<RESET<< endl;
    }
}
void staffOptions() {
    int choice;
    do {
        cout <<CYAN<< "STAFF OPTIONs" <<RESET<< endl;
        cout << "  " << endl;
        cout << "1. View All Current Appointments" << endl;
        cout << "2. Update Appointment Status:" << endl;
        cout << "3. Alter Doctor Time Slots/Add Doctor" << endl;
        cout << "4. Display All User Credentials:" << endl;
        cout << "5. Log Out" << endl;
        cout << " " << endl;
        cout << "Enter your choice: ";

        if (!(cin >> choice)) {
            clearInputBuffer();
            choice = 0;
            continue;
        }
        clearInputBuffer();

        switch (choice) {
        case 1: viewAllAppointments(); break;
        case 2: updateAppointmentStatus(); break;
        case 3: alterDoctorInfo(); break;
        case 4: displayAllUsers(); break;
        case 5: cout <<YELLOW<< "[INFO] Logging out staff portal." <<RESET<< endl; break;
        default: cout <<RED<< "[ALERT] Invalid choice. Please try again." <<RESET<< endl; break;
        }
    } while (choice != 5);
}
void viewAllAppointments() {
    cout << CYAN<<"\nALL RECENT APPOINTMENTS" <<RESET<< endl;
    if (appointments.empty()) {
        cout <<YELLOW<< "[INFO] No appointments have been booked yet."<<RESET << endl;
        return;
    }
    displayAppointmentTable(appointments);
}
void updateAppointmentStatus() {
    cout << CYAN<<setw(12)<<"\n UPDATE APPOINTMENT STATUS" << RESET<<endl;
    int tokenId;
    cout << "Enter Token ID to confirm status (e.g., Complete): ";
    if (!(cin >> tokenId)) {
        clearInputBuffer();
        cout << RED<<"[ALERT] Invalid input." <<RESET<< endl;
        return;
    }
    clearInputBuffer();
    bool found = false;
    for (size_t i = 0; i < appointments.size(); ++i) {
        if (appointments[i].token == tokenId) {
            appointments[i].status = "Completed";
            cout <<GREEN<< "\n[SUCCESS] Appointment Token " << tokenId << " status updated to Completed." <<RESET<< endl;
            found = true;
            break;
        }
    }

    if (!found) {
        cout <<RED<< "[ALERT] Token ID not found." <<RESET<< endl;
    }
}
void alterDoctorInfo() {
    int choice;
    cout <<CYAN<<setw(12)<< "\nALTER DOCTOR INFORMATION" <<RESET<< endl;
    cout << "1. Add New Doctor" << endl;
    cout << "2. View/Edit Doctor Info:" << endl;
    cout << "Enter choice: ";

    if (!(cin >> choice)) {
        clearInputBuffer();
        cout <<RED<< "[ALERT] Invalid input." <<RESET<< endl;
        return;
    }
    clearInputBuffer();

    if (choice == 1) {
        Doctor newDoc;
        newDoc.id = doctors.empty() ? 101 : doctors.back().id + 1;

        cout << "New Doctor ID: " << newDoc.id << endl;
        cout << "Enter Doctor Name: ";
        getline(cin, newDoc.name);
        cout << "Enter Qualification: ";
        getline(cin, newDoc.qualification);
        cout << "Enter Fee: ";
        while (!(cin >> newDoc.fee)) {
            clearInputBuffer();
            cout << RED<<"[ALERT] Invalid fee. Enter a number: "<<RESET;
        }
        clearInputBuffer();
        cout << "Using default time slots: 9:00, 10:00, 11:00, 14:00, 15:00." << endl;
        newDoc.timeSlots[0] = "9:00";
        newDoc.timeSlots[1] = "10:00";
        newDoc.timeSlots[2] = "11:00";
        newDoc.timeSlots[3] = "14:00";
        newDoc.timeSlots[4] = "15:00";

        doctors.push_back(newDoc);
        ofstream outFile(DOCTOR_FILE);
        if (outFile.is_open()) {
            for (const auto& doc : doctors) {
                outFile << doc.id << "\n" << doc.name << "\n" << doc.qualification << "\n" << doc.fee;
                for (int i = 0; i < 5; ++i) {
                    outFile << "\n" << doc.timeSlots[i];
                }
                outFile << "\n";
            }
            outFile.close();
            cout <<GREEN<< "\n[SUCCESS] New Doctor " << newDoc.name << " added successfully." <<RESET<< endl;
        }
        else {
            cout << YELLOW<<"[ERROR] Unable to open doctor file for saving." <<RESET<< endl;
        }
    }
    else {
        cout << YELLOW<<"[INFO] Only 'Add New Doctor' is implemented for this demonstration." <<RESET<< endl;
    }
}
void displayAllUsers() {
    cout <<CYAN<<setw(12)<< "\nALL REGISTERED USERS" << RESET<<endl;
    if (users.empty()) {
        cout <<YELLOW<< "[INFO] No users registered yet." << RESET<<endl;
        return;
    }
    cout << setfill('-') << setw(85) << "-" << endl;
    cout << setfill(' ') << left
        << "| " << setw(20) << "Name"
        << "| " << setw(14) << "CNIC"
        << "| " << setw(12) << "Contact"
        << "| " << setw(7) << "Gender"
        << "| " << setw(20) << "Password"
        << "|" << endl;
    cout << setfill('-') << setw(85) << "-" << endl;
    for (const auto& user : users) {
        cout << setfill(' ') << left
            << "| " << setw(20) << user.name
            << "| " << setw(14) << user.cnic
            << "| " << setw(12) << user.contact
            << "| " << setw(7) << user.gender
            << "| " << setw(20) << "*****"
            << "|" << endl;
    }

    cout << setfill('-') << setw(85) << "-" << endl;
}
void loadData() {
    loadDoctors();
    ifstream userIn(USER_FILE);
    if (userIn.is_open()) {
        User u;
        while (getline(userIn, u.name) &&
            getline(userIn, u.cnic) &&
            getline(userIn, u.contact) &&
            userIn >> u.gender &&
            userIn.ignore() && 
            getline(userIn, u.password)) {
            users.push_back(u);
        }
        userIn.close();
    }
    ifstream aptIn(APPOINTMENT_FILE);
    if (aptIn.is_open()) {
        Appointment a;
        while (aptIn >> a.token &&
            aptIn.ignore() &&
            getline(aptIn, a.userCnic) &&
            aptIn >> a.doctorId &&
            aptIn.ignore() &&
            getline(aptIn, a.timeSlot) &&
            getline(aptIn, a.status)) {
            appointments.push_back(a);
        }
        aptIn.close();
    }
}
void saveUsers() {
    ofstream outFile(USER_FILE);
    if (outFile.is_open()) {
        for (const auto& user : users) {
            outFile << user.name << "\n"
                << user.cnic << "\n"
                << user.contact << "\n"
                << user.gender << "\n"
                << user.password << "\n";
        }
        outFile.close();
    }
    else {
        cout << RED<<"[ERROR] Unable to save user data to file." <<RESET<< endl;
    }
}
void saveAppointments() {
    ofstream outFile(APPOINTMENT_FILE);
    if (outFile.is_open()) {
        for (const auto& apt : appointments) {
            outFile << apt.token << "\n"
                << apt.userCnic << "\n"
                << apt.doctorId << "\n"
                << apt.timeSlot << "\n"
                << apt.status << "\n";
        }
        outFile.close();
    }
    else {
        cout <<RED<< "[ERROR] Unable to save appointment data to file." <<RESET<< endl;
    }
}
void loadDoctors() {
    doctors.clear();
    ifstream docIn(DOCTOR_FILE);
    if (docIn.is_open()) {
        Doctor d;
        while (docIn >> d.id && docIn.ignore() &&
            getline(docIn, d.name) &&
            getline(docIn, d.qualification) &&
            docIn >> d.fee && docIn.ignore()) {
            for (int i = 0; i < 5; ++i) {
                getline(docIn, d.timeSlots[i]);
            }
            doctors.push_back(d);
        }
        docIn.close();
    }
}
void populateDefaultDoctors() {
    doctors = {
        {101, "Dr. Ali Khan", "MBBS, Internal Medicine", 1500.00, {"9:00", "10:00", "11:00", "14:00", "15:00"}},
        {102, "Dr. Sara Ahmed", "BDS, General Dentistry", 1000.00, {"9:30", "10:30", "11:30", "14:30", "15:30"}},
        {102, "Dr. Syeda Amna", "BDS, General Dentistry", 2000.00, {"9:30", "10:30", "11:30", "14:30", "15:30"}}
    };

    ofstream outFile(DOCTOR_FILE);
    if (outFile.is_open()) {
        for (const auto& doc : doctors) {
            outFile << doc.id << "\n" << doc.name << "\n" << doc.qualification << "\n" << doc.fee;
            for (int i = 0; i < 5; ++i) {
                outFile << "\n" << doc.timeSlots[i];
            }
            outFile << "\n";
        }
        outFile.close();
        cout << YELLOW<<"[INFO] Default doctors loaded and saved." <<RESET<< endl;
    }
    else {
        cout <<RED<< "[ERROR] Unable to save default doctor data." <<RESET<< endl;
    }
}
bool isCnicValid(const string& cnic) {
    if (cnic.length() != 13) return false;
    for (char c : cnic) {
        if (!isdigit(c)) return false;
    }
    return true;
}
bool isCnicUnique(const string& cnic) {
    for (const auto& user : users) {
        if (user.cnic == cnic) {
            return false;
        }
    }
    return true;
}

bool isContactValid(const string& contact) {
    if (contact.length() != 11) return false;
    for (char c : contact) {
        if (!isdigit(c)) return false;
    }
    return true;
}

bool isGenderValid(char gender) {
    return (gender == 'M' || gender == 'F' || gender == 'X');
}
void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void displayDoctorTable() {
    cout <<CYAN<<setw(12)<< " AVAILABLE DOCTORS, QUALIFICATIONS & FEES" << RESET<<endl;
    cout << " " << endl;
    cout << setfill('-') << setw(77) << "-" << endl;
    cout << setfill(' ') << left
        << "| " << setw(5) << "ID"
        << "| " << setw(20) << "Name"
        << "| " << setw(30) << "Qualification"
        << "| " << setw(10) << "Fee (PKR)"
        << "|" << endl;
    cout << setfill('-') << setw(77) << "-" << endl;

    for (const auto& doc : doctors) {
        cout << setfill(' ') << left
            << "| " << setw(5) << doc.id
            << "| " << setw(20) << doc.name
            << "| " << setw(30) << doc.qualification
            << "| " << setw(10) << fixed << setprecision(2) << doc.fee
            << "|" << endl;
    }

    cout << setfill('-') << setw(77) << "-" << endl;
}

void displayAppointmentTable(const vector<Appointment>& apts) {
    cout << setfill('-') << setw(85) << "-" << endl;
    cout << setfill(' ') << left
        << "| " << setw(7) << "Token"
        << "| " << setw(14) << "User CNIC"
        << "| " << setw(10) << "Doctor ID"
        << "| " << setw(10) << "Time"
        << "| " << setw(15) << "Doctor Name"
        << "| " << setw(12) << "Status"
        << "|" << endl;
    cout << setfill('-') << setw(85) << "-" << endl;

    for (const auto& apt : apts) {
        string docName = "Unknown";
        for (const auto& doc : doctors) {
            if (doc.id == apt.doctorId) {
                docName = doc.name;
                break;
            }
        }

        cout << setfill(' ') << left
            << "| " << setw(7) << apt.token
            << "| " << setw(14) << apt.userCnic
            << "| " << setw(10) << apt.doctorId
            << "| " << setw(10) << apt.timeSlot
            << "| " << setw(15) << docName
            << "| " << setw(12) << apt.status
            << "|" << endl;
    }
    cout << setfill('-') << setw(85) << "-" << endl;
}
int generateToken() {
    int maxToken = 1000;
    for (const auto& apt : appointments) {
        if (apt.token > maxToken) {
            maxToken = apt.token;
        }
    }
    return maxToken + 1;
}
