/**********************************************************
 * Course: CS213 - Programming II  - 2019                 *
 * Title:  Assignment I                                   *
 * Group:  G1 (Old)                                       *
 * Team members:                                          *
 *   Abdelrahman Nasr Abdelsalam         -   20170343     *
 *   Bahaa El-din Osama Sayed            -   20170078     *
 *   Karim Nour El-din Saleh             -   20170200     *
 **********************************************************/
#include <bits/stdc++.h>
using namespace std;


string spacing = "     ";
int numOfUsers = 1;
string myChoice;
int tempId = 0; /// an out-of-scope watcher of question id when setting a question to prevent duplication


class Question
{
protected:
    string que;
    string ans;
    int id;

public:
    /// to set the question
    void setQue(string q){
        que = q;
        tempId++; /// to give a unique id for each question created
        id = tempId;
    }

    void setAns(string a){ans = a;} /// answer setter
    string getAns(){return ans;} /// answer getter

    /// to delete id of useless objects
    void deleteId(){tempId--;}

    /// question getter
    string getQue(){return que;}

    int getId(){return id;}

    /// to set the question from file (it's overridden by subclasses)
    void readQuestionFromFile(string a[]){
        this -> setQue(a[0]);
    }

    /// to check if the question is already exist
    bool operator==(Question &x) {
        if(this -> getQue() == x.getQue()){
            return true;
        }

        return false;
    }
};


class MCQ : public Question
{
public:
    string choices[4]; /// carries the 4 choices of the MCQ

    /// reading the 4 choices of MCQ
    void readQuestionFromFile(string a[]){
        Question::readQuestionFromFile(a);

        for(int i=1; i<5; i++){
            if(i == 1) ans = a[1];
            choices[i-1] = a[i];
        }
    }

};


class TF : public Question
{
public:
    /// reading the answer of TF question
    void readQuestionFromFile(string a[]){
        Question::readQuestionFromFile(a);

        ans = a[1];
    }
};


class Complete : public Question
{
public:
    /// reading the answer of Complete question
    void readQuestionFromFile(string a[]){
        Question::readQuestionFromFile(a);

        ans = a[1];
    }
};

/// important part.
/// exist 3 types of questions.
///*********************************************************************************************************************
/// [1]MCQ Questions.
int MCQ_capacity = 0;
MCQ *MCQ_questions = new MCQ[MCQ_capacity];  /// Dynamic array to carry MCQ questions and answers.
///---------------------------------------------------------------------------------------------------------------------
/// [2]True or False Questions.
int TF_capacity = 0;
TF *TF_questions = new TF[TF_capacity];  /// Dynamic array to carry TF questions and answers.
///---------------------------------------------------------------------------------------------------------------------
/// [3]Complete Questions.
int Complete_capacity = 0;
Complete *Complete_questions = new Complete[Complete_capacity];  /// Dynamic array to carry Complete questions and answers.
///*************************************************************************************************************************



void login(); /// the login screen

/// the parent of Admin & Player class which holds the data of the user
/// there's an array of objects called allUsers used to gather all the users
/// in one place to make the access to the user data more easy


class User
{
protected:
    string firstName, lastName, username, role, password;
    string choose;
    int sub_capacity=0;
    int *stat = new int[sub_capacity];

public:
    /// setters
    void user_info(string a, string b, string c, string d, string e){
        firstName   = a;
        lastName    = b;
        username    = c;
        role        = d;
        password    = e;
    }

    /// takes the choice from the user and returns it (often used for options)
    string choice() {
        cout << endl << "My choice: ";
        cin >> choose;
        cout << "\n------------------------------------------------------------------\n" << endl;

        return choose;
    }

    /// getters
    string get_firstName(){return firstName;}
    string get_lastName(){return lastName;}
    string get_username(){return username;}
    string get_password(){return password;}
    string get_role(){return role;}

    /// setters
    void set_firstName(string p){firstName = p;}
    void set_lastName(string p){lastName = p;}
    void set_password(string p){password = p;}

    /// common options between admin & player users
    void switch_accounts(){login();}
    void exit_game(){exit(0);}
};

/// Dynamic array of objects to store all users data in the program
User *allUsers = new User[numOfUsers];

void expand_allUsers(); /// expands allUsers array to make it ready to receive a new user
void admin_menu(User& x); /// displays the admin menu options and redirect each option to Admin class method
void player_menu(User& a); /// displays the player menu options and redirect each option to Player class method

void update_data(User& u); /// (karim) - update your name option




/// expanding the array of questions to be ready to receive a new question
template<typename T>
T* expand(T *arrOfQuestions, int& capacity){
    T *temp = arrOfQuestions;
    arrOfQuestions = new T[capacity + 1];
    if(capacity > 0)
    {
        for (int i = 0; i < capacity; ++i) {
            arrOfQuestions[i] = temp[i];
        }
    }
    capacity++; /// to keep track of size
    delete[] temp;
    return arrOfQuestions;
}


/// shrinking the array of questions to delete THE LAST question
template<typename T>
T* shrink(T *arrOfQuestions, int& capacity){
    T *temp = arrOfQuestions;
    arrOfQuestions = new T[capacity - 1];

    if(capacity > 0){
        for (int i = 0; i < capacity-1; ++i) {
            arrOfQuestions[i] = temp[i];
        }
    }
    capacity--; /// to keep track of size
    delete[] temp;
    return arrOfQuestions;
}



/// deleting A SPECIFIC question by it's id from the array of questions
template<typename T>
T* deleteSpecQue(T *arrOfQuestions, int& capacity, int q_id, int& tempSuccess){
    for(int i=0; i < capacity; i++){
        if(q_id == arrOfQuestions[i].getId() && capacity > 1){

            T *temp = arrOfQuestions;
            arrOfQuestions = new T[capacity - 1];

            int n = 0;

            for (int j = 0; j < capacity; ++j) {
                if (j == i || (j == i && i == 0)) {
                    j += 1; /// To make a gap that ignore the deleted question
                    n = 1; /// To reduce the gap between iterators
                }
                if(j < capacity) arrOfQuestions[j - n] = temp[j];
            }

            capacity -= 1; /// To keep track of size

            delete[] temp;

            tempSuccess = 1; /// To alert success

            break;

        }

        else if (q_id == arrOfQuestions[i].getId() && capacity == 1) { /// when there is only one question
            arrOfQuestions = new T[capacity - 1];
            capacity -= 1;

            tempSuccess = 1; /// To alert success

            break;
        }
    }

    if(tempSuccess != 1) tempSuccess = 0;

    return arrOfQuestions;
}

class Admin : public User
{
private:
    string data; /// needed in TakeAndAdd()
    string info[5] = {
            "Your question: ",
            "choice 1 (the correct choice): ",
            "choice 2: ",
            "choice 3: ",
            "choice 4: "};

    int queId;
    int success = 2; /// a watcher of progress & errors
    string tempChoice;

    int counter; /// needed in viewQue() & loadQue()

    fstream datafile; /// needed in loadQue()
    string filename;
    int nameLen;
    bool check;
    string c;
    string dataFromFile[5]; /// used to hold the lines of the question from the file and pass it to the setters

public:
    /// it takes the question type the user wants to add and process it
    void TakeAndAdd(string que_type) {

        if(que_type == "1"){
            MCQ_questions = expand(MCQ_questions, MCQ_capacity);

            ///*** Taking the question from the user ***///
            cin.ignore();

            for (int i = 0; i < 5; i++) {
                cout << info[i];
                getline(cin, data);

                if (i == 0){
                    MCQ_questions[MCQ_capacity - 1].setQue(data); /// to store the question

                    for(int j=0; j<MCQ_capacity - 1; j++){ /// check that the question isn't duplicated
                        if(MCQ_questions[MCQ_capacity - 1] == MCQ_questions[j]){
                            success = 0; /// to inform the others that the question is duplicated and it needs to be deleted
                            break;
                        }
                    }

                    if(success == 0) break;
                }
                else{ /// if not duplicated then store the answers
                    if (i == 1){
                        MCQ_questions[MCQ_capacity - 1].setAns(data); /// set the right answer
                        MCQ_questions[MCQ_capacity - 1].choices[0] = "*" + data; /// put star to the right answer
                    } else {
                        MCQ_questions[MCQ_capacity - 1].choices[i-1] = data; /// store the choices in the choices array
                    }
                }
            }
            if(success == 0){ /// deleting the duplicated question
                MCQ_questions[MCQ_capacity - 1].deleteId(); /// so this useless question addition doesn't affect the id arrangement
                MCQ_questions = shrink(MCQ_questions, MCQ_capacity); /// delete the last question (the duplicated one)
            }
        }

        if(que_type == "2"){

            TF_questions = expand(TF_questions, TF_capacity);

            ///*** Taking the question from the user ***///
            cin.ignore();

            cout << "Your question: ";
            getline(cin, data);
            TF_questions[TF_capacity - 1].setQue(data);

            for(int j=0; j<TF_capacity - 1; j++){ /// check that the question isn't duplicated
                if(TF_questions[TF_capacity - 1] == TF_questions[j]){
                    success = 0;
                    break;
                }
            }

            if(success == 0){
                TF_questions[TF_capacity - 1].deleteId(); /// so this useless question addition doesn't affect the id arrangement
                TF_questions = shrink(TF_questions, TF_capacity);
            } else {
                cout << "Question's Answer: ";
                getline(cin, data);

                /// error that the answer should be true or false
                if(data != "true" && data != "false"){

                    cout << "\n==========================================\n";
                    cout << "= THE ANSWER SHOULD BE \"true\" OR \"false\" =\n";
                    cout << "==========================================\n";

                    TF_questions[TF_capacity - 1].deleteId(); /// so this useless question addition doesn't affect the id arrangement
                    TF_questions = shrink(TF_questions, TF_capacity);

                    success = 1;
                } else {
                    TF_questions[TF_capacity - 1].setAns(data);
                }

            }
        }

        if(que_type == "3"){

            Complete_questions = expand(Complete_questions, Complete_capacity);

            ///*** Taking the question from the user ***///
            cin.ignore();

            cout << "Your question: ";
            getline(cin, data);

            Complete_questions[Complete_capacity - 1].setQue(data);

            for(int j=0; j<Complete_capacity - 1; j++){ /// check that the question isn't duplicated
                if(Complete_questions[Complete_capacity - 1] == Complete_questions[j]){
                    success = 0;
                    break;
                }
            }

            if(success == 0){
                Complete_questions[Complete_capacity - 1].deleteId(); /// so this useless question addition doesn't affect the id arrangement
                Complete_questions = shrink(Complete_questions, Complete_capacity);
            } else {
                cout << "Question's Answer: ";
                getline(cin, data);
                Complete_questions[Complete_capacity - 1].setAns(data);
            }
        }

        if(success == 0){
            cout << "\n==================================\n";
            cout << "= THIS QUESTION IS ALREADY EXIST =\n";
            cout << "==================================\n";
        } else if (success == 2) {
            cout << "\n=======================================\n";
            cout << "= YOUR QUESTION IS ADDED SUCCESSFULLY =\n";
            cout << "=======================================\n";
        }

        success = 2; /// reset
    }

    /// adding the question by the admin
    void AddQue(User y) {
        cout << "Choose your question type from the following:" << endl;
        cout << spacing << "[1] MCQ" << endl;
        cout << spacing << "[2] True/False" << endl;
        cout << spacing << "[3] Complete" << endl;

        myChoice = choice();

        if      (myChoice == "1") TakeAndAdd(myChoice); /// redirect to TakeAndAdd() to complete the process
        else if (myChoice == "2") TakeAndAdd(myChoice);
        else if (myChoice == "3") TakeAndAdd(myChoice);
        else{
            cout << "====================" << endl;
            cout << "= Wrong choice! :( =" << endl;
            cout << "====================" << endl;
        }

        admin_menu(y); /// if the question is added or not .. redirect to admin menu
    }

    /// viewing all questions added from all types( either it's added or loaded from a file)
    void viewQue(User y){

        int all_question_capacity = MCQ_capacity + TF_capacity + Complete_capacity; /// just a shortcut
        cout << "Number of questions available: " << all_question_capacity << endl;
        cout << "--------------------------------\n\n";

        /// if there's at least one question then display the questions
        if (all_question_capacity > 0) {
            cout << "MC Questions list (Total: " << MCQ_capacity << " Questions)" << endl;
            cout << "--------------------------------------" << endl;

            for (int i = 0; i < MCQ_capacity; i++) {
                tempChoice = "";
                cout << "[" << i+1 << "] " << "(ID: " << MCQ_questions[i].getId() << ") ";
                cout << MCQ_questions[i].getQue() << endl;

                for(int j=0; j < 4; j++){
                    tempChoice = "";
                    tempChoice += "[";
                    tempChoice += (char)('a' + j);
                    tempChoice += "] ";

                    tempChoice += MCQ_questions[i].choices[j];

                    cout << spacing;
                    cout << left << setfill(' ') << setw(20);   /// TO arrangement choices.
                    cout << tempChoice;
                }
                cout << endl << endl;
            }

            cout << "--------------------------------------\n\n";
            cout << "TF Questions list (Total: " << TF_capacity << " Questions)" << endl;
            cout << "--------------------------------------" << endl;

            for(int i=0; i < TF_capacity; i++){
                cout << "[" << i+1 << "] " << "(ID: " << TF_questions[i].getId() << ") ";
                cout << TF_questions[i].getQue() << " ";
                cout << "(Answer: " << TF_questions[i].getAns() << ")" << endl;
                cout << endl;
            }

            cout << "--------------------------------------\n\n";
            cout << "Complete Questions list (Total: " << Complete_capacity << " Questions)" << endl;
            cout << "--------------------------------------------" << endl;

            for(int i=0; i < Complete_capacity; i++){
                cout << "[" << i+1 << "] " << "(ID: " << Complete_questions[i].getId() << ") ";
                cout << Complete_questions[i].getQue() << " ";
                cout << "(Answer: " << Complete_questions[i].getAns() << ")" << endl;
                cout << endl;
            }

        } else {
            cout << "\n=======================================\n";
            cout << "= There are no questions to be viewed =\n";
            cout << "=   Please, add/load some questions   =\n";
            cout << "=======================================\n\n";

            admin_menu(y); /// redirect
        }

        cout << "------------------------------------------------------------------\n" << endl;

        /// Alert messages for deletion process
        if (success == 1) {
            cout << "\n========================================\n";
            cout << "= The question is deleted successfully =\n";
            cout << "========================================\n\n";

            success = 2; /// reset tracker value
        } else if (success == 0) {
            cout << "\n=============================================\n";
            cout << "= Sorry, The question ID is out of range :( =\n";
            cout << "=============================================\n\n";

            success = 2; /// reset tracker value
        }

        /// Delete a question
        cout << "Press [d] and the question ID if you want to delete a question (Example: d 2)" << endl;
        cout << "Press [b] if you want to go back to the admin menu" << endl;

        myChoice = choice(); /// takes the user choice

        if (myChoice == "d") { /// if the admin wants to delete a question
            cin >> queId; /// take the question id that admin wants it to be deleted

            /// passing the question id to the template to delete it
            MCQ_questions = deleteSpecQue(MCQ_questions, MCQ_capacity, queId, success);
            TF_questions = deleteSpecQue(TF_questions, TF_capacity, queId, success);
            Complete_questions = deleteSpecQue(Complete_questions, Complete_capacity, queId, success);

            viewQue(y); /// display all questions after the deletion process
        } else if (myChoice == "b") admin_menu(y);
        else { /// if choice is incorrect then display error

            cout << "==============================\n";
            cout << "= Error! Incorrect choice :( =\n";
            cout << "==============================\n\n";

            cin.clear();
            fflush(stdin);

            viewQue(y); /// redirect
        }

    }

    /// load questions from file
    void loadQue(User y){
        cout << "File name: ";
        cin.ignore();
        getline(cin, filename); /// taking the filename

        nameLen = filename.length(); /// calculating the file name length

        /// place extension if it doesn't exist
        if (nameLen <= 4 || filename.substr(nameLen - 4) != ".txt") filename += ".txt";

        datafile.open(filename.c_str(), ios::in); /// open the file

        /// if there's a problem in the file display an error and exit the process
        if (datafile.fail()) {
            cout << "\n=============================\n";
            cerr << "= Error! File not found! :( =\n";
            cout << "=============================\n\n";
            admin_menu(y); /// redirect
        }

        check = true; /// because the beginning of the file always has strange character

        /// while reading the file
        while (!datafile.eof()) {
            getline(datafile, c); /// take each line and store to c variable

            /// if it's the first line of file and there's a problem check if there's no garbage in it
            /// maybe the problem is because of the first 3 strange characters
            if(check && c[0] != 'M' && c[0] != 'T' && c[0] != 'C'){
                c.erase(0, 3); /// get rid of the first 3 characters
                check = false; /// don't do this in the next lines
            }

            /// if the type is known then continue the process
            if(c == "MCQ" || c == "TF" || c == "COMPLETE"){
                /// if it's MCQ then get the upcoming 5 lines (question + 4 choices) and store it
                if(c == "MCQ"){
                    MCQ_questions = expand(MCQ_questions, MCQ_capacity);

                    for(int i=0; i < 5; i++){
                        getline(datafile, dataFromFile[i]);

                        /// if there's a line contains a question type then there's a problem in this question structure
                        if(dataFromFile[i] == "MCQ" || dataFromFile[i] == "TF" || dataFromFile[i] == "Complete"){

                            /// delete last question added to MCQ_questions
                            MCQ_questions = shrink(MCQ_questions, MCQ_capacity);

                            success = 0; /// report failure

                            break;

                        } else { /// if everything is okay then put a star before the correct answer
                            if(i==1){dataFromFile[1] = "*" + dataFromFile[1];}
                        }
                    }

                    if (success == 0){
                        cout << "\n==========================================\n";
                        cout << "=    THERE IS A PROBLEM IN YOUR FILE!    =\n";
                        cout << "=    AN ERROR OCCURED IN A MC Question   =\n";
                        cout << "=   PLEASE, CHECK YOUR FILE STRUCTURE    =\n";
                        cout << "==========================================\n\n";

                        success = 2;
                        break;

                    } else {
                        MCQ_questions[MCQ_capacity-1].readQuestionFromFile(dataFromFile);
                    }
                }
                else if(c == "TF"){

                    TF_questions = expand(TF_questions, TF_capacity);

                    for(int i=0; i < 2; i++){
                        getline(datafile, dataFromFile[i]);

                        /// if the second line (answer) in TF question type s stored and not true or false then error
                        if( i==1 && dataFromFile[1] != "true" && dataFromFile[1] != "false"){

                            /// delete last question added to TF_questions
                            TF_questions = shrink(TF_questions, TF_capacity);

                            success = 0;

                            break;

                        }
                    }

                    if (success == 0){
                        cout << "\n==========================================\n";
                        cout << "=    THERE IS A PROBLEM IN YOUR FILE!    =\n";
                        cout << "=    AN ERROR OCCURED IN A TF Question   =\n";
                        cout << "=   PLEASE, CHECK YOUR FILE STRUCTURE    =\n";
                        cout << "==========================================\n\n";

                        success = 2;
                        break;

                    } else {
                        TF_questions[TF_capacity-1].readQuestionFromFile(dataFromFile);
                    }
                }
                else if(c == "COMPLETE"){

                    Complete_questions = expand(Complete_questions, Complete_capacity);

                    for(int i=0; i < 2; i++){
                        getline(datafile, dataFromFile[i]);

                        /// if the second line in Complete question type is blank then error
                        if(i==1 && (dataFromFile[i] == "MCQ" || dataFromFile[i] == "TF" || dataFromFile[i] == "Complete")){

                            /// delete last question added to Complete_questions
                            Complete_questions = shrink(Complete_questions, Complete_capacity);

                            success = 0;

                            break;

                        }
                    }

                    if (success == 0){
                        cout << "\n===========================================\n";
                        cout << "=    THERE IS A PROBLEM IN YOUR FILE!     =\n";
                        cout << "= AN ERROR OCCURED IN A COMPLETE Question =\n";
                        cout << "=    PLEASE, CHECK YOUR FILE STRUCTURE    =\n";
                        cout << "===========================================\n\n";

                        success = 2;
                        break;

                    } else {
                        Complete_questions[Complete_capacity-1].readQuestionFromFile(dataFromFile);
                    }
                }

                success = 1; /// to alert success
            }

                /// if there's a type not supported halt the loading
            else {
                cout << "\n=====================================================\n";
                cout << "=       THERE IS A PROBLEM IN A QUESTION TYPE!      =\n";
                cout << "= THE PROGRAM ONLY SUPPORTS (MCQ/TF/COMPLETE) TYPES =\n";
                cout << "=====================================================\n\n";

                success = 0; /// to alert failure

                break;
            }
        }

        if(success == 1){
            cout << "\n==========================================\n";
            cout << "= THE FILE HAS BEEN LOADED SUCCESSFULLY! =\n";
            cout << "==========================================\n\n";

            success = 2; /// reset
        }

        datafile.close();

        admin_menu(y);
    }
};


class Quiz
{
protected :
    string correct_choice;
    string myanswer;


    int Big_Capacity = MCQ_capacity + TF_capacity + Complete_capacity;


    int correct_MCQ=0;
    int correct_TF=0;
    int correct_complete=0;


    int choices_capacity=4;


    /// This is all questions in pool.
    /// MCQ questions = 8.
    /// TF questions = 2.
    /// Complete questions = 3.


    /// by default
    int cnt_MCQ=2;/// 2*1=2
    int cnt_TF=1;/// 1*2=2
    int cnt_Complete=2;  /// 2*3=6
    int questions_per_quiz = cnt_MCQ + cnt_TF + cnt_Complete ;

public:
    string answer();
    void newQuiz(User& u);
    void randomize(int arr[] , int n);
    void generate_random_questions(int question_per_quiz);

};



/// class[8].
class Player : public User , public Quiz
{
public:
        double avg = 0;
        double sum = 0;

    void scoreStat()
    {
        /*int *temp = stat;
        stat = new int[sub_capacity +1]; /// Expand by one

        /// Give the values back to stat array
        for (int i = 0; i < sub_capacity; ++i) {
            stat[i] = temp[i];
        }
       // sub_capacity++; /// To keep track of the number of quizzes taken

        delete[] temp;
       */
        /// If number of quizzes taken = 1 then store in index 0 the number of correct answer for quiz 1
        stat[sub_capacity] = (correct_MCQ*1+correct_TF*2+correct_complete*3);
        sub_capacity++;
        for (int h = 0; h < sub_capacity; h++)
        {
            sum += stat[h];
        }
        avg = sum / sub_capacity;
        /******************************/
        if (sub_capacity == 0) {

            cout << "\n=================================================\n";
            cout << "= Sorry, You should at least solve 1 quiz first =\n";
            cout << "=================================================\n\n";


        } else {
            int high = stat[0]; /// Assuming high score is at the first quiz
            int low = stat[0]; /// Assuming low score is at the first quiz

            /// Making a boundary so it compares when there are two values or more
            if (sub_capacity > 1) {

                /// Get highest score
                for (int j = 1; j < sub_capacity; ++j) {
                    if (high < stat[j])

                        high = stat[j];

                }

                /// Get lowest score
                for (int k = 1; k < sub_capacity; ++k) {
                    if (low > stat[k])

                        low = stat[k];

                }
            }

            cout << "Your score statistics: " << endl;
            cout << spacing << "- Number of Quizzes taken: " << sub_capacity << endl;
            cout << spacing << "- Highest score:           " << high << endl;
            cout << spacing << "- Lowest score:            " << low << endl;
            cout << spacing << "- Average score:           " << avg << "\n\n";
        }


    }


};




int main()
{

    /// Default user initialization
    allUsers[numOfUsers-1].user_info("abdelrahman", "nasr", "n", "admin", "n");
    expand_allUsers(); /// To make the array ready to receive another user
    allUsers[numOfUsers-1].user_info("bahaa", "osama", "b", "player", "b");


    login();


    return 0;
}


void login(){
    string EnteredUsername, EnteredPassword;

    bool check = true; /// to keep in displaying login screen until the user enter valid data

    while(check) {
        cout << "**************************************" << endl;
        cout << "Welcome to the Quiz game program V2.0!" << endl;
        cout << "**************************************" << endl;

        cout << endl;

        cout << "Username: ";
        cin >> EnteredUsername;

        cout << "Password: ";
        cin >> EnteredPassword;

        /// searching in all users and redirect the user (if it's found) to a suitable menu
        for(int i=0; i < numOfUsers; i++){
            if(EnteredUsername == allUsers[i].get_username() && EnteredPassword == allUsers[i].get_password()) {
                if(allUsers[i].get_role() == "admin") admin_menu(allUsers[i]);
                else player_menu(allUsers[i]);

                check = false; /// to break the while loop
                break; /// to stop looking for this user
            }
        }

        if(check){
            /// to display an error if data entered is wrong
            cout << endl;
            cout << "==============================" << endl;
            cout << "= Wrong username or password =" << endl;
            cout << "==============================" << endl;
            cout << endl;
        }
    }
}

void expand_allUsers(){

    User *temp = allUsers;
    allUsers = new User[numOfUsers + 1];

    for (int i = 0; i < numOfUsers; ++i) {
        allUsers[i] = temp[i];
    }
    numOfUsers++;

    delete[] temp;
}

/// Update your name option
void update_data(User& u)
{

    string fname ,lname ,passwd;

    cout << "What do you want to update?" << endl;
    cout << spacing << "[1] First name" << endl;
    cout << spacing << "[2] Last name" << endl;
    cout << spacing << "[3] Password" << endl;

    myChoice = u.choice();

    if(myChoice == "1"){
        cout <<"New first name: ";
        cin >> fname;
        u.set_firstName(fname);
    }
    else if (myChoice == "2"){
        cout <<"New last name: ";
        cin >> lname;
        u.set_lastName(lname);
    }
    else if (myChoice == "3"){
        cout <<"New password: ";
        cin >> passwd;
        u.set_password(passwd);
    }
    else {
        cout << "\n====================================" << endl;
        cout << "= Wrong input .. please try again! =" << endl;
        cout << "===================================\n" << endl;
        update_data(u);
    }

    if(u.get_role() == "admin") admin_menu(u);
    else player_menu(u);
}


void admin_menu(User& x){
    Admin root;

    string menuItems[8] = {
            "Switch accounts",
            "Update your name",
            "View all users",
            "Add new user",
            "View all questions",
            "Add new question",
            "Load questions from file",
            "Exit"};

    cout << endl;
    cout << "Welcome " << x.get_firstName() << " " << x.get_lastName() << " (" << x.get_role() << "), please choose from the following options:" << endl;

    for (int i = 0; i < sizeof(menuItems) / sizeof(menuItems[0]); i++) {
        cout << spacing << "[" << i + 1 << "] " << menuItems[i] << endl;
    }

    myChoice = root.choice();

    if      (myChoice == "1") root.switch_accounts();
    else if (myChoice == "2") update_data(x);
    else if (myChoice == "5") root.viewQue(x);
    else if (myChoice == "6") root.AddQue(x);
    else if (myChoice == "7") root.loadQue(x);
    else if (myChoice == "8") root.exit_game();
}


void player_menu(User& a){
    Player plyr;

    string menuItems[7] = {
            "Switch accounts",
            "Update your name",
            "Start a new quiz",
            "Display your scores statistics",
            "Display all your scores",
            "Display details of your last 2 quizzes",
            "Exit"};

    cout << endl;
    cout << "Welcome " << a.get_firstName() << " " << a.get_lastName() << " (" << a.get_role() << "), please choose from the following options:" << endl;

    for (int i = 0; i < sizeof(menuItems) / sizeof(menuItems[0]); i++) {
        cout << spacing << "[" << i + 1 << "] " << menuItems[i] << endl;
    }

    myChoice = plyr.choice();

    if      (myChoice == "1") plyr.switch_accounts();
    else if (myChoice == "2") update_data(a);
    else if (myChoice == "3") plyr.newQuiz(a);
    else if (myChoice == "4") plyr.scoreStat();
    else if (myChoice == "7") plyr.exit_game();

}




/// General idea of randomize :i change places of indexes.
void Quiz::randomize(int arr[] , int n)
{

    srand(time(nullptr));
    int j = 0 ;
    for(int i=n-1 ; i>0;i--)
    {
        j = rand()%(i);  /// from n-1 to 0  .
        swap(arr[i],arr[j] );
    }

}

void Quiz::newQuiz(User& u)
{
    if(Big_Capacity < questions_per_quiz )
    {
        cout << "\n\n" ;
        cout << "***************************************************************************************************************" << "\n" ;
        cout << "WRONG!" << "\n";
        cout << "The questions in file not enough to create new quiz."  << "\n";
        cout << "***************************************************************************************************************" << "\n" ;
        cout << "\n\n";
    }
    else if(cnt_MCQ > MCQ_capacity || cnt_TF >TF_capacity || cnt_Complete >Complete_capacity )
    {
        cout << "*****************************************************************" << "\n";
        cout << "WRONG!" << "\n";
        cout << "initialization of MCQ ,TF or Complete greater than capacity." << "\n";
        cout << "*****************************************************************" << "\n";
    }

    else
    {
        generate_random_questions(questions_per_quiz);
    }

    /// Go to player menu.
    player_menu(u);
}

void Quiz::generate_random_questions(int question_per_quiz)
{
    int arr_choices[4] = {4,1,3,2};

    int*arr_mcq      = new int [MCQ_capacity];
    int*arr_TF       = new int [TF_capacity];
    int*arr_Complete = new int [Complete_capacity];


    for(int i=0;i<MCQ_capacity;++i){arr_mcq[i]=i;}
    for(int i=0;i<TF_capacity;++i){arr_TF[i]=i;}
    for(int i=0;i<Complete_capacity;++i){arr_Complete[i]=i;}


    randomize(arr_mcq ,MCQ_capacity);
    randomize(arr_choices ,choices_capacity);
    randomize(arr_TF ,TF_capacity);
    randomize(arr_Complete ,Complete_capacity);


    cout << "*************" << "\n";
    cout << "Question [1] " << "\n";
    cout << "MCQ          " << "\n";
    cout << "*************" << "\n";
    cout << "\n\n";

    /// MCQ.
    for (int i = 0; i < cnt_MCQ; i++)
    {
        string  tempChoice = "";
        cout << "[" << i+1 << "]"<< MCQ_questions[arr_mcq[i]].getQue() << endl;

        for(int j=0; j < 4; j++)
        {

            tempChoice = "";
            tempChoice += "[";
            tempChoice += (char)('a' + j);
            tempChoice += "] ";

            tempChoice += MCQ_questions[arr_mcq[i]].choices[arr_choices[j]-1]  ;

            if(tempChoice[4] == '*' )
            {
                correct_choice = tempChoice[1] ;   /// store correct choice .
                tempChoice.erase(tempChoice.begin() + 4);

            }
            cout << spacing;
            cout << left << setfill(' ') << setw(20);   /// TO arrangement choices.
            cout << tempChoice;


        }

        myanswer = answer();
        if( myanswer == correct_choice )
        {
            ++correct_MCQ ;
        }
        randomize(arr_choices ,choices_capacity);

        cout << "\n";
    }


    cout << "**************" << "\n";
    cout << "Question [2]  " << "\n";
    cout << "True and False" << "\n";
    cout << "**************" << "\n";
    cout << "\n\n";

/// TF.
    for(int i=0 ; i<cnt_TF;++i)
    {

        TF:
        cout << "[" << i+cnt_MCQ+1 << "] " << TF_questions[arr_TF[i]].getQue() <<"\n";

        myanswer = answer();

        if( myanswer == TF_questions[arr_TF[i]].getAns() )
        {
            ++correct_TF ;
        }  cout << "\n\n";

        cout << "\n";
    }



    cout << "**************" << "\n";
    cout << "Question [3]  " << "\n";
    cout << "Complete      " << "\n";
    cout << "**************" << "\n";
    cout << "\n\n";

/// Complete.
    for(int i=0;i<cnt_Complete;++i)
    {
        cout << "[" << i + cnt_MCQ + cnt_TF + 1 << "] "<<Complete_questions[arr_Complete[i]].getQue()<< "\n";

        myanswer = answer();
        if(myanswer == Complete_questions[arr_Complete[i]].getAns())
        {
            ++correct_complete ;
        }

        cout << "\n";
    }

    cout << "correct_MCQ = " <<correct_MCQ<<"\n";
    cout << "correct_TF = " <<correct_TF<<"\n";
    cout << "correct_complete = " <<correct_complete<<"\n";



    cout << "************************************************"  << "\n";
    cout << "Your Score: (" << (correct_MCQ*1+correct_TF*2+correct_complete*3)<< "/10)"<< "\n";
    cout << "************************************************"  << "\n";

}

string Quiz::answer()
{
    cout << "\n";
    cout << "Enter your answer : " ;
    cin  >> myanswer;
    cout << "\n";

    return myanswer ;
}
