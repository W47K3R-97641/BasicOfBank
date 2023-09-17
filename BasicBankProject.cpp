#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;

void Login();
void GoBackToMainMenue();
void ShowMainMenue();
void ShowTransactionsMenue();
void ShowManageUsersMenue();

struct sClient
{
    string AccountNumber = "";
    string PinCode = "";
    string Name = "";
    string Phone = "";
    double AccountBalance;
    bool MarkForDelete = false;
};
struct sUser
{
    string UserName = "";
    string Password = "";
    int  Permissions = 0;
    bool MarkForDelete = false;
};

sUser CurrentUser;

const string ClientsFileName = "Clients.txt";
const string UsersFileName = "Users.txt";

enum enTransactionsMenueOptions { eDeposit = 1, eWithdraw = 2, eShowTotalBalance = 3, eShowMainMenue = 4 };
enum enMainMenueOptions { eListClients = 1, eAddNewClient = 2, eDeleteClient = 3, eUpdateClient = 4, eFindClient = 5, eShowTransactionsMenue = 6, eManageUsers = 7, eLogout = 8 };
enum enMainMenuePermissions { pAll = -1, pListClients = 1, pAddNewClient = 2, pDeleteClient = 4, pUpdateClient = 8, pFindClient = 16, pShowTransactionsMenue = 32, pManageUsers = 64 };
enum enManageUsersMenue { eListUser = 1, eAddNewUser = 2, eDeleteUser = 3, eUpdateUser = 4, eFindUser = 5, eMainMenue = 6 };


vector<string> SplitString(string S1, string Delim)
{

    vector<string> vString;

    short pos = 0;
    string sWord; // define a string variable  

    // use find() function to get the position of the delimiters  
    while ((pos = S1.find(Delim)) != std::string::npos)
    {
        sWord = S1.substr(0, pos); // store the word   
        if (sWord != "")
        {
            vString.push_back(sWord);
        }

        S1.erase(0, pos + Delim.length());  /* erase() until positon and move to next word. */
    }

    if (S1 != "")
    {
        vString.push_back(S1); // it adds last word of the string.
    }

    return vString;

}

void Logout()
{
    sUser ClearUser;
    CurrentUser = ClearUser;
    Login();
}

string ConvertRecordToLine(sClient Client, string Seperator = "#//#")
{

    string stClientRecord = "";

    stClientRecord += Client.AccountNumber + Seperator;
    stClientRecord += Client.PinCode + Seperator;
    stClientRecord += Client.Name + Seperator;
    stClientRecord += Client.Phone + Seperator;
    stClientRecord += to_string(Client.AccountBalance);

    return stClientRecord;

}
string ConvertUserToLine(sUser User, string Seperator = "#//#")
{
    string RecordLine = "";

    RecordLine += User.UserName + Seperator;
    RecordLine += User.Password + Seperator;
    RecordLine += to_string(User.Permissions);

    return RecordLine;
}

sClient ConvertLinetoRecord(string Line, string Seperator = "#//#")
{

    sClient Client;
    vector<string> vClientData;

    vClientData = SplitString(Line, Seperator);

    Client.AccountNumber = vClientData[0];
    Client.PinCode = vClientData[1];
    Client.Name = vClientData[2];
    Client.Phone = vClientData[3];
    Client.AccountBalance = stod(vClientData[4]);//cast string to double


    return Client;

}
sUser   ConvertLineToUser(string Line)
{
    vector <string> vUsersData = SplitString(Line, "#//#");
    sUser User;

    User.UserName = vUsersData[0];
    User.Password = vUsersData[1];
    User.Permissions = stoi(vUsersData[2]);

    return User;
}

void PrintClientRecord(sClient Client)
{

    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(10) << left << Client.PinCode;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.Phone;
    cout << "| " << setw(12) << left << Client.AccountBalance;

}
void PrintUserRecord(sUser User)
{
    cout << "| " << setw(15) << left << User.UserName;
    cout << "| " << setw(40) << left << User.Password;
    cout << "| " << setw(12) << left << User.Permissions;
}

vector <sClient> LoadCleintsDataFromFile(string FileName)
{

    vector <sClient> vClients;

    fstream MyFile;
    MyFile.open(FileName, ios::in);//read Mode

    if (MyFile.is_open())
    {

        string Line;
        sClient Client;

        while (getline(MyFile, Line))
        {

            Client = ConvertLinetoRecord(Line);

            vClients.push_back(Client);
        }

        MyFile.close();

    }

    return vClients;

}
vector <sUser>   LoadUsersDataFromFile(string FileName)
{
    vector <sUser> vUsers;

    fstream MyFile;
    MyFile.open(FileName, ios::in);//read Mode


    if (MyFile.is_open())
    {

        string Line = "";
        sUser User;
        while (getline(MyFile, Line))
        {

            User = ConvertLineToUser(Line);

            vUsers.push_back(User);
            Line = "";
        }

        MyFile.close();

    }

    return vUsers;
}

vector <sClient> SaveCleintsDataToFile(string FileName, vector <sClient> vClients)
{

    fstream MyFile;
    MyFile.open(FileName, ios::out);//overwrite

    string DataLine;

    if (MyFile.is_open())
    {

        for (sClient C : vClients)
        {

            if (C.MarkForDelete == false)
            {
                //we only write records that are not marked for delete.  
                DataLine = ConvertRecordToLine(C);
                MyFile << DataLine << endl;

            }

        }

        MyFile.close();

    }

    return vClients;

}

void AddDataLineToFile(string FileName, string  stDataLine)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out | ios::app);

    if (MyFile.is_open())
    {

        MyFile << stDataLine << endl;

        MyFile.close();
    }

}
void SaveUsersDataToFile(string FileName, vector <sUser> vUsers)
{

    fstream MyFile;
    MyFile.open(FileName, ios::out);//overwrite

    string DataLine;

    if (MyFile.is_open())
    {

        for (sUser currUser : vUsers)
        {

            if (currUser.MarkForDelete == false)
            {
                //we only write records that are not marked for delete.  
                DataLine = ConvertUserToLine(currUser);
                MyFile << DataLine << endl;
            }

        }

        MyFile.close();

    }

}

void PrintUserCard(sUser User)
{
    cout << "\nThe following are the User details:\n";
    cout << "-----------------------------------";
    cout << "\nUsername    : " << User.UserName;
    cout << "\nPassword    : " << User.Password;
    cout << "\nPermissions : " << User.Permissions;
    cout << "\n-----------------------------------\n";

}
void PrintClientCard(sClient Client)
{
    cout << "\nThe following are the client details:\n";
    cout << "-----------------------------------";
    cout << "\nAccout Number: " << Client.AccountNumber;
    cout << "\nPin Code     : " << Client.PinCode;
    cout << "\nName         : " << Client.Name;
    cout << "\nPhone        : " << Client.Phone;
    cout << "\nAccount Balance: " << Client.AccountBalance;
    cout << "\n-----------------------------------\n";

}
void PrintClientRecordBalanceLine(sClient Client)
{

    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.AccountBalance;

}

string ReadClientAccountNumber()
{
    string AccountNumber = "";

    cout << "\nPlease enter AccountNumber? ";
    cin >> AccountNumber;
    return AccountNumber;

}
string ReadUsername(string Message)
{
    string username = "";
    cout << Message;
    getline(cin >> ws, username);
    return username;
}
string ReadPassword(string Message)
{
    string Password = "";
    cout << Message;
    getline(cin >> ws, Password);
    return Password;
}

bool FindClientByAccountNumber(string AccountNumber, vector <sClient> vClients, sClient& Client)
{

    for (sClient C : vClients)
    {

        if (C.AccountNumber == AccountNumber)
        {
            Client = C;
            return true;
        }

    }
    return false;

}
bool FindUserByUsernameAndPassword(string Username, string Password, sUser Users)
{
    vector <sUser> vUsers = LoadUsersDataFromFile(UsersFileName);

    for (sUser& User : vUsers)
    {
        if (User.UserName == Username && User.Password == Password)
        {
            CurrentUser = User;
            return true;
        }
    }
    return false;
}
bool FindUserByUsername(string UserName, vector <sUser> vUsers, sUser& User)
{

    for (sUser currUser : vUsers)
    {
        if (currUser.UserName == UserName)
        {
            User = currUser;
            return true;
        }
    }

    return false;
}


bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{

    for (sClient& C : vClients)
    {

        if (C.AccountNumber == AccountNumber)
        {
            C.MarkForDelete = true;
            return true;
        }

    }

    return false;

}
bool MarkUserForDeleteByUsername(string UsernameToDelete, vector <sUser>& vUsers)
{
    for (sUser& currUser : vUsers)
    {
        if (currUser.UserName == UsernameToDelete)
        {
            currUser.MarkForDelete = true;
            return true;
        }

    }
    return false;

}

bool DeleteClientByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{

    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {

        PrintClientCard(Client);

        cout << "\n\nAre you sure you want delete this client? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
            SaveCleintsDataToFile(ClientsFileName, vClients);

            //Refresh Clients 
            vClients = LoadCleintsDataFromFile(ClientsFileName);

            cout << "\n\nClient Deleted Successfully.";
            return true;
        }

    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
    }

    return false;
}
bool DeleteUserByUsername(string UsernameToDelete, vector <sUser>& vUsers)
{
    sUser User;
    char Answer = 'N';

    if (UsernameToDelete == "Admin")
    {
        cout << "\n\n You Can't Delete This User. :(";
        return false;
    }

    if (FindUserByUsername(UsernameToDelete, vUsers, User))
    {
        PrintUserCard(User);

        cout << "\n\n Are You Sure You Want To Delete This User";
        cin >> Answer;

        if (toupper(Answer) == 'Y')
        {
            MarkUserForDeleteByUsername(UsernameToDelete, vUsers);

            SaveUsersDataToFile(UsersFileName, vUsers);

            vUsers = LoadUsersDataFromFile(UsersFileName);

            cout << "\n\nUser Deleted Successfully.";
            return true;
        }
    }
    else
    {
        cout << "\n\n User With Username [" << UsernameToDelete << "]Not Found!!";
    }
    return false;
}

bool ClientExistsByAccountNumber(string AccountNumber, string FileName)
{
    vector <sClient> vClients;

    fstream MyFile;
    MyFile.open(FileName, ios::in);//read Mode

    if (MyFile.is_open())
    {

        string Line;
        sClient Client;

        while (getline(MyFile, Line))
        {

            Client = ConvertLinetoRecord(Line);
            if (Client.AccountNumber == AccountNumber)
            {
                MyFile.close();
                return true;
            }


            vClients.push_back(Client);
        }

        MyFile.close();

    }

    return false;


}
bool UserExistsByUsername(string UserName, vector <sUser> vUsers)
{
    for (sUser& User : vUsers)
    {
        if (User.UserName == UserName)
            return true;
    }
    return false;
}


int ReadPermissions()
{
    char GiveAccess = 'Y';
    cout << "\n\nDo You Want To Give Full Access ? y/n ? ";
    cin >> GiveAccess;

    if (toupper(GiveAccess) == 'Y')
    {
        return enMainMenuePermissions::pAll;
    }

    int AccessTo = 0;

    cout << "\n\nDo You Want To Give Access to :";

    cout << "\nShow Client List ? y/n ? ";
    cin >> GiveAccess;
    AccessTo += toupper(GiveAccess) == 'Y' ? enMainMenuePermissions::pListClients : AccessTo;

    cout << "\nAdd New Client ? y/n ? ";
    cin >> GiveAccess;
    AccessTo += toupper(GiveAccess) == 'Y' ? enMainMenuePermissions::pAddNewClient : AccessTo;

    cout << "\nDelete Client ? y/n ? ";
    cin >> GiveAccess;
    AccessTo += toupper(GiveAccess) == 'Y' ? enMainMenuePermissions::pDeleteClient : AccessTo;

    cout << "\nUpdate Client ? y/n ? ";
    cin >> GiveAccess;
    AccessTo += toupper(GiveAccess) == 'Y' ? enMainMenuePermissions::pUpdateClient : AccessTo;

    cout << "\nFind Client ? y/n ? ";
    cin >> GiveAccess;
    AccessTo += toupper(GiveAccess) == 'Y' ? enMainMenuePermissions::pFindClient : AccessTo;

    cout << "\nTransactions ? y/n ? ";
    cin >> GiveAccess;
    AccessTo += toupper(GiveAccess) == 'Y' ? enMainMenuePermissions::pShowTransactionsMenue : AccessTo;

    cout << "\nManage Users ? y/n ? ";
    cin >> GiveAccess;
    AccessTo += toupper(GiveAccess) == 'Y' ? enMainMenuePermissions::pManageUsers : AccessTo;

    return AccessTo;
}

sClient ReadNewClient()
{
    sClient Client;

    cout << "Enter Account Number? ";

    // Usage of std::ws will extract allthe whitespace character
    getline(cin >> ws, Client.AccountNumber);

    while (ClientExistsByAccountNumber(Client.AccountNumber, ClientsFileName))
    {
        cout << "\nClient with [" << Client.AccountNumber << "] already exists, Enter another Account Number? ";
        getline(cin >> ws, Client.AccountNumber);
    }


    cout << "Enter PinCode? ";
    getline(cin, Client.PinCode);

    cout << "Enter Name? ";
    getline(cin, Client.Name);

    cout << "Enter Phone? ";
    getline(cin, Client.Phone);

    cout << "Enter AccountBalance? ";
    cin >> Client.AccountBalance;

    return Client;

}
sUser ReadNewUser()
{
    vector <sUser> vUsers = LoadUsersDataFromFile(UsersFileName);

    sUser User;

    User.UserName = ReadUsername("\nEnter Username : ");

    while (UserExistsByUsername(User.UserName, vUsers))
    {
        cout << "\nUser With [" << User.UserName << "Already Exists :(, Try Another Username";
        User.UserName = ReadUsername("\nEnter Username : ");
    }

    User.Password = ReadPassword("\nEnter Password : ");

    User.Permissions = ReadPermissions();

    return User;
}

void AddNewClient()
{
    sClient Client;
    Client = ReadNewClient();
    AddDataLineToFile(ClientsFileName, ConvertRecordToLine(Client));

}
void AddNewClients()
{
    char AddMore = 'Y';
    do
    {
        //system("cls");
        cout << "Adding New Client:\n\n";

        AddNewClient();
        cout << "\nClient Added Successfully, do you want to add more clients? Y/N? ";


        cin >> AddMore;

    } while (toupper(AddMore) == 'Y');

}
void AddNewUser()
{
    sUser User;
    User = ReadNewUser();
    AddDataLineToFile(UsersFileName, ConvertUserToLine(User));
}
void AddNewUsers()
{
    char AddMore = 'Y';

    do
    {
        cout << "Adding New User:\n\n";

        AddNewUser();
        cout << "\nUser Added Successfully, do you want to add more User? Y/N? ";
        cin >> AddMore;

    } while (toupper(AddMore) == 'Y');
}

sClient ChangeClientRecord(string AccountNumber)
{
    sClient Client;

    Client.AccountNumber = AccountNumber;

    cout << "\n\nEnter PinCode? ";
    getline(cin >> ws, Client.PinCode);

    cout << "Enter Name? ";
    getline(cin, Client.Name);

    cout << "Enter Phone? ";
    getline(cin, Client.Phone);

    cout << "Enter AccountBalance? ";
    cin >> Client.AccountBalance;

    return Client;

}
sUser ChangeUserRecord(string Username)
{
    sUser User;
    User.UserName = Username;

    User.Password = ReadPassword("\nEnter Password : ");

    User.Permissions = ReadPermissions();

    return User;
}

bool DepositBalanceToClientByAccountNumber(string AccountNumber, double Amount, vector <sClient>& vClients)
{


    char Answer = 'n';


    cout << "\n\nAre you sure you want perfrom this transaction? y/n ? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {

        for (sClient& C : vClients)
        {
            if (C.AccountNumber == AccountNumber)
            {
                C.AccountBalance += Amount;
                SaveCleintsDataToFile(ClientsFileName, vClients);
                cout << "\n\nDone Successfully. New balance is: " << C.AccountBalance;

                return true;
            }

        }


    }

    return false;
}

bool UpdateClientByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{

    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {

        PrintClientCard(Client);
        cout << "\n\nAre you sure you want update this client? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {

            for (sClient& C : vClients)
            {
                if (C.AccountNumber == AccountNumber)
                {
                    C = ChangeClientRecord(AccountNumber);
                    break;
                }

            }

            SaveCleintsDataToFile(ClientsFileName, vClients);

            cout << "\n\nClient Updated Successfully.";
            return true;
        }

    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
    }

    return false;
}
bool UpdateUserByUsername(string UsernameToUpdate, vector <sUser>& vUsers)
{
    sUser User;
    char Answer = 'N';

    if (UsernameToUpdate == "Admin")
    {
        cout << "\n\n You Can't Update This User. :(";
        return false;
    }

    if (FindUserByUsername(UsernameToUpdate, vUsers, User))
    {
        PrintUserCard(User);

        cout << "\n\n Are You Sure You Want To Update This User ? y/n ?";
        cin >> Answer;

        if (toupper(Answer) == 'Y')
        {

            for (sUser& currUser : vUsers)
            {
                if (currUser.UserName == UsernameToUpdate)
                {
                    currUser = ChangeUserRecord(UsernameToUpdate);
                    break;
                }
            }

            SaveUsersDataToFile(UsersFileName, vUsers);

            cout << "\n\nUser Updated Successfully.";

            return true;
        }
    }
    else
    {
        cout << "\n\n User With Username [" << UsernameToUpdate << "] Not Found!!";
    }
    return false;
}

bool CheckAccessPermission(enMainMenuePermissions Permission)
{
    if (CurrentUser.Permissions == enMainMenuePermissions::pAll)
        return true;

    if ((Permission & CurrentUser.Permissions) == Permission)
        return true;

    return false;
}

void ShowAccessDeniedMessage()
{
    cout << "-------------------------------------------\n";
    cout << "\nAccess Denied, :(";
    cout << "\nYou Don't Have Permission To Do This,";
    cout << "\nPlease Contact Your Admin";
    cout << "\n-------------------------------------------\n";
}

short ReadMainMenueOption()
{
    cout << "Choose what do you want to do? [1 to 8]? ";
    short Choice = 0;
    cin >> Choice;

    return Choice;
}
short ReadManageMenueOption()
{
    cout << "Choose what do you want to do? [1 to 6]? ";
    short Choice = 0;
    cin >> Choice;

    return Choice;
}
short ReadTransactionsMenueOption()
{
    cout << "Choose what do you want to do? [1 to 4]? ";
    short Choice = 0;
    cin >> Choice;

    return Choice;
}

void ShowAllClientsScreen()
{
    if (!CheckAccessPermission(enMainMenuePermissions::pListClients))
    {
        system("cls");
        ShowAccessDeniedMessage();
        GoBackToMainMenue();
    }

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);

    cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(10) << "Pin Code";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Phone";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    if (vClients.size() == 0)
        cout << "\t\t\t\tNo Clients Available In the System!";
    else

        for (sClient Client : vClients)
        { 

            PrintClientRecord(Client);
            cout << endl;
        }

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

}
void ShowAddNewClientsScreen()
{
    if (!CheckAccessPermission(enMainMenuePermissions::pAddNewClient))
    {
        system("cls");
        ShowAccessDeniedMessage();
        GoBackToMainMenue();
    }

    cout << "\n-----------------------------------\n";
    cout << "\tAdd New Clients Screen";
    cout << "\n-----------------------------------\n";

    AddNewClients();

}
void ShowDeleteClientScreen()
{
    if (!CheckAccessPermission(enMainMenuePermissions::pDeleteClient))
    {
        system("cls");
        ShowAccessDeniedMessage();
        GoBackToMainMenue();
    }
    cout << "\n-----------------------------------\n";
    cout << "\tDelete Client Screen";
    cout << "\n-----------------------------------\n";

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    DeleteClientByAccountNumber(AccountNumber, vClients);

}
void ShowUpdateClientScreen()
{
    if (!CheckAccessPermission(enMainMenuePermissions::pUpdateClient))
    {
        system("cls");
        ShowAccessDeniedMessage();
        GoBackToMainMenue();
    }
    cout << "\n-----------------------------------\n";
    cout << "\tUpdate Client Info Screen";
    cout << "\n-----------------------------------\n";

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    UpdateClientByAccountNumber(AccountNumber, vClients);

}
void ShowFindClientScreen()
{
    if (!CheckAccessPermission(enMainMenuePermissions::pFindClient))
    {
        system("cls");
        ShowAccessDeniedMessage();
        GoBackToMainMenue();
    }

    cout << "\n-----------------------------------\n";
    cout << "\tFind Client Screen";
    cout << "\n-----------------------------------\n";

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    sClient Client;
    string AccountNumber = ReadClientAccountNumber();
    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
        PrintClientCard(Client);
    else
        cout << "\nClient with Account Number[" << AccountNumber << "] is not found!";

}

void ShowListUsersScreen()
{
    vector <sUser> vUsers = LoadUsersDataFromFile(UsersFileName);

    cout << "\n\t\t\t\t\tUsers List (" << vUsers.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "| " << left << setw(15) << "Username";
    cout << "| " << left << setw(40) << "Password";
    cout << "| " << left << setw(12) << "Permission";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    if (vUsers.size() == 0)
        cout << "\t\t\t\tNo Users Available In the System!";
    else

        for (sUser User : vUsers)
        {

            PrintUserRecord(User);
            cout << endl;
        }
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
}
void ShowAddNewUsersScreen()
{
    cout << "===========================================\n";
    cout << "\t\tAdd New Users Screen\n";
    cout << "===========================================\n";
    AddNewUsers();

}
void ShowDeleteUserScreen()
{
    cout << "===========================================\n";
    cout << "\t\tDelete User Screen\n";
    cout << "===========================================\n";

    vector <sUser> vUsers = LoadUsersDataFromFile(UsersFileName);
    string UsernameToDelete = ReadUsername("Enter Username You Want To Delete ? ");
    DeleteUserByUsername(UsernameToDelete, vUsers);

}
void ShowUpdateUserScreen()
{
    cout << "===========================================\n";
    cout << "\t\tUpdate User Screen\n";
    cout << "===========================================\n";

    vector <sUser> vUsers = LoadUsersDataFromFile(UsersFileName);
    string UsernameToUpdate = ReadUsername("Enter Username You Want To Update ? ");

    UpdateUserByUsername(UsernameToUpdate, vUsers);

}
void ShowFindUserScreen()
{
    cout << "===========================================\n";
    cout << "\t\tFind User Screen\n";
    cout << "===========================================\n";

    vector <sUser> vUsers = LoadUsersDataFromFile(UsersFileName);
    string UsernameToFind = ReadUsername("Enter Username You Want To Find ? ");
    sUser User;
    if (FindUserByUsername(UsernameToFind, vUsers, User))
    {
        PrintUserCard(User);
    }
    else
    {
        cout << "\n\n User With Username [" << UsernameToFind << "] Not Found!!";
    }

}

void ShowTotalBalances()
{

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);

    cout << "\n\t\t\t\t\tBalances List (" << vClients.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    double TotalBalances = 0;

    if (vClients.size() == 0)
        cout << "\t\t\t\tNo Clients Available In the System!";
    else

        for (sClient Client : vClients)
        {

            PrintClientRecordBalanceLine(Client);
            TotalBalances += Client.AccountBalance;

            cout << endl;
        }

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    cout << "\t\t\t\t\t   Total Balances = " << TotalBalances;

}
void ShowDepositScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tDeposit Screen";
    cout << "\n-----------------------------------\n";


    sClient Client;

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();


    while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        cout << "\nClient with [" << AccountNumber << "] does not exist.\n";
        AccountNumber = ReadClientAccountNumber();
    }


    PrintClientCard(Client);

    double Amount = 0;
    cout << "\nPlease enter deposit amount? ";
    cin >> Amount;

    DepositBalanceToClientByAccountNumber(AccountNumber, Amount, vClients);

}
void ShowWithDrawScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tWithdraw Screen";
    cout << "\n-----------------------------------\n";

    sClient Client;

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();


    while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        cout << "\nClient with [" << AccountNumber << "] does not exist.\n";
        AccountNumber = ReadClientAccountNumber();
    }

    PrintClientCard(Client);

    double Amount = 0;
    cout << "\nPlease enter withdraw amount? ";
    cin >> Amount;

    //Validate that the amount does not exceeds the balance
    while (Amount > Client.AccountBalance)
    {
        cout << "\nAmount Exceeds the balance, you can withdraw up to : " << Client.AccountBalance << endl;
        cout << "Please enter another amount? ";
        cin >> Amount;
    }

    DepositBalanceToClientByAccountNumber(AccountNumber, Amount * -1, vClients);

}
void ShowTotalBalancesScreen()
{

    ShowTotalBalances();

}

void GoBackToManageUserMenue()
{
    cout << "\n\nPress any key to go back To Manage User Menue...";
    system("pause>0");
    ShowManageUsersMenue();
}
void GoBackToMainMenue()
{
    cout << "\n\nPress any key to go back to Main Menue...";
    system("pause>0");
    ShowMainMenue();
}
void GoBackToTransactionsMenue()
{
    cout << "\n\nPress any key to go back to Transactions Menue...";
    system("pause>0");
    ShowTransactionsMenue();

}

void PerfromMainMenueOption(enMainMenueOptions MainMenueOption)
{

    switch (MainMenueOption)
    {
    case enMainMenueOptions::eListClients:
    {
        system("cls");
        ShowAllClientsScreen();
        GoBackToMainMenue();
        break;
    }
    case enMainMenueOptions::eAddNewClient:
        system("cls");
        ShowAddNewClientsScreen();
        GoBackToMainMenue();
        break;

    case enMainMenueOptions::eDeleteClient:
        system("cls");
        ShowDeleteClientScreen();
        GoBackToMainMenue();
        break;

    case enMainMenueOptions::eUpdateClient:
        system("cls");
        ShowUpdateClientScreen();
        GoBackToMainMenue();
        break;

    case enMainMenueOptions::eFindClient:
        system("cls");
        ShowFindClientScreen();
        GoBackToMainMenue();
        break;

    case enMainMenueOptions::eShowTransactionsMenue:
        system("cls");
        ShowTransactionsMenue();
        break;

    case enMainMenueOptions::eManageUsers:
        system("cls");
        ShowManageUsersMenue();
        break;

    case enMainMenueOptions::eLogout:
        Logout();
        break;
    }

}
void PerfromManageUsersMenue(enManageUsersMenue ManageMenueOption)
{
    switch (ManageMenueOption)
    {
    case enManageUsersMenue::eListUser:
        system("cls");
        ShowListUsersScreen();
        GoBackToManageUserMenue();
        break;
    case enManageUsersMenue::eAddNewUser:
        system("cls");
        ShowAddNewUsersScreen();
        GoBackToManageUserMenue();
        break;
    case enManageUsersMenue::eDeleteUser:
        system("cls");
        ShowDeleteUserScreen();
        GoBackToManageUserMenue();
        break;
    case enManageUsersMenue::eUpdateUser:
        system("cls");
        ShowUpdateUserScreen();
        GoBackToManageUserMenue();
        break;
    case enManageUsersMenue::eFindUser:
        system("cls");
        ShowFindUserScreen();
        GoBackToManageUserMenue();
        break;
    case enManageUsersMenue::eMainMenue:
        ShowMainMenue();
        break;
    }
}
void PerfromTranactionsMenueOption(enTransactionsMenueOptions TransactionMenueOption)
{
    switch (TransactionMenueOption)
    {
    case enTransactionsMenueOptions::eDeposit:
    {
        system("cls");
        ShowDepositScreen();
        GoBackToTransactionsMenue();
        break;
    }

    case enTransactionsMenueOptions::eWithdraw:
    {
        system("cls");
        ShowWithDrawScreen();
        GoBackToTransactionsMenue();
        break;
    }


    case enTransactionsMenueOptions::eShowTotalBalance:
    {
        system("cls");
        ShowTotalBalancesScreen();
        GoBackToTransactionsMenue();
        break;
    }


    case enTransactionsMenueOptions::eShowMainMenue:
    {
        ShowMainMenue();
    }
    }

}

void ShowMainMenue()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tMain Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Show Client List.\n";
    cout << "\t[2] Add New Client.\n";
    cout << "\t[3] Delete Client.\n";
    cout << "\t[4] Update Client Info.\n";
    cout << "\t[5] Find Client.\n";
    cout << "\t[6] Transactions.\n";
    cout << "\t[7] Manage Users.\n";
    cout << "\t[8] Logout.\n";
    cout << "===========================================\n";

    PerfromMainMenueOption((enMainMenueOptions)ReadMainMenueOption());
}
void ShowManageUsersMenue()
{
    if (!CheckAccessPermission(enMainMenuePermissions::pManageUsers))
    {
        system("cls");
        ShowAccessDeniedMessage();
        GoBackToMainMenue();
    }
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tManage Users Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] List Users.\n";
    cout << "\t[2] Add New User.\n";
    cout << "\t[3] Delete User.\n";
    cout << "\t[4] Update User.\n";
    cout << "\t[5] Find User.\n";
    cout << "\t[6] Main Menue.\n";
    cout << "===========================================\n";
    PerfromManageUsersMenue((enManageUsersMenue)ReadManageMenueOption());
}
void ShowTransactionsMenue()
{
    if (!CheckAccessPermission(enMainMenuePermissions::pShowTransactionsMenue))
    {
        system("cls");
        ShowAccessDeniedMessage();
        GoBackToMainMenue();
    }
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tTransactions Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Deposit.\n";
    cout << "\t[2] Withdraw.\n";
    cout << "\t[3] Total Balances.\n";
    cout << "\t[4] Main Menue.\n";
    cout << "===========================================\n";
    PerfromTranactionsMenueOption((enTransactionsMenueOptions)ReadTransactionsMenueOption());
}

bool LoadUserInfo(string Username, string Password) {
    if (FindUserByUsernameAndPassword(Username, Password, CurrentUser))
        return true;
    else
        return false;

}

void Login()
{
    bool LoginFaild = false;
    do
    {
        system("cls");
        cout << "===========================================\n";
        cout << "\t\tLogin Screen\n";
        cout << "===========================================\n";

        if (LoginFaild)
            cout << "\nInvalid Username/Password :( !!.";

        string Username = ReadUsername("\nEnter Username : ");
        string Password = ReadPassword("\nEnter Password : ");

        LoginFaild = !LoadUserInfo(Username, Password);

    } while (LoginFaild);

    ShowMainMenue();
}

int main()
{
    Login();
    return 0;
}