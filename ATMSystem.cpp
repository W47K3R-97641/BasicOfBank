#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <iomanip>
using namespace std;

void Login();
void GoBackToMainMenue();
void ShowMainMenue();
void ShowQuickWithdrawScreen();

struct sClient
{
    string AccountNumber = "";
    string PinCode = "";
    string Name = "";
    string Phone = "";
    bool MarkForDelete = false;
    double AccountBalance;
};

sClient CurrentClient;

const string ClientsFileName = "Clients.txt";

enum enMainMenueOptions {eQuickWithdraw = 1, eNormalWithdraw = 2, eDeposit = 3, eCheckBalance = 4, eLogout = 5};


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
string ReadClientAccountNumber(string Message)
{
    string AccountNumber = "";
    cout << Message;
    cin >> AccountNumber;
    return AccountNumber;

}
string ReadClientPinCode(string Message)
{
    string PinCode = "";
    cout << Message;
    cin >> PinCode;
    return PinCode;
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
bool FindClientByAccountNumberAndPinCode(string AccountNumber, string PinCode, sClient& Clear_sClient)
{
    vector <sClient> vClient = LoadCleintsDataFromFile(ClientsFileName);
    for (sClient& Client : vClient)
    {
        if (Client.AccountNumber == AccountNumber && Client.AccountNumber == AccountNumber)
        {
            Clear_sClient = Client;
            return true;
        }
    }
    return false;
}
bool LoadClientInfo(string AccountNumber, string PinCode)
{
    if (FindClientByAccountNumberAndPinCode(AccountNumber, PinCode, CurrentClient))
        return true;
    else
        return false;

}
short ReadMainMenueOption()
{
    short Choice = 0;
    bool ReadFaild = false;
    do
    {
        if (ReadFaild)
            cout << "\nOption Not Exist :( !!.\n";
        
        cout << "Choose what do you want to do? [1 to 5]? ";
        cin >> Choice;
        
        ReadFaild = Choice < 1 || Choice > 5;
    } while (ReadFaild);
    
    return Choice;
}
short ReadQuickWithdrawOption()
{
    short Choice = 0;
    bool ReadFaild = false;
    do
    {
        if (ReadFaild)
            cout << "\nOption Not Exist :( !!.\n";
        
        cout << "Choose what You Want To Do From [1] to [8] ? ";
        cin >> Choice;
        
        ReadFaild = Choice < 1 || Choice > 9;
    } while (ReadFaild);
    return Choice;
}

bool DepositBalanceToClientByAccountNumber(string AccountNumber, double Amount)
{
    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    
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
void Withdraw(double Amount)
{
    DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, Amount * -1);
    CurrentClient.AccountBalance -= Amount;
}
short GetQuickWithdrawAmount(short QuickWithdrawOption)
{

    switch (QuickWithdrawOption)
    {
    case 1 : return 20;
    case 2 : return 50;
    case 3 : return 100;
    case 4 : return 200;
    case 5 : return 400;
    case 6 : return 600;
    case 7 : return 800;
    case 8 : return 1000;
    default: return 0;
    }
}
void PerfromQuickWithdraw(short QuickWithdrawOption)
{
    if (QuickWithdrawOption == 9)
        return;

    short Amount = GetQuickWithdrawAmount(QuickWithdrawOption);
    

    if (Amount > CurrentClient.AccountBalance)
    {
        cout << "\nThe Amount Exceeds Your Balance, Make Another Choice ";
        cout << "\n\nPress Anykey Yo Continue....";
        system("pause>0");
        ShowQuickWithdrawScreen();
    }
    else
    {
        Withdraw(Amount);
    }
}
double ReadWithdrawAmount(string Message)
{
    double Amount = 0;
    bool NotValidAmount = false;

    do
    {
        cout << Message;
        cin >> Amount;
        NotValidAmount = fmod(Amount, 5) || (Amount <= 0);
    }while(NotValidAmount);

    return Amount; 
}
double ReadDepositAmount(string Message)
{
    double Amount = 0;
    bool NotValidAmount = false;

    do
    {
        if (NotValidAmount)
            cout << "\nAmount Not Valid Please enter Amount From 1$ To Above\n";
        cout << Message;
        cin >> Amount;
        NotValidAmount = Amount <= 0;
    }while(NotValidAmount);

    return Amount;
}

void ShowQuickWithdrawScreen()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tQuick Withdraw\n";
    cout << "===========================================\n";
    cout << "\t[1]20$\t[2]50$\n\t[3]100$\t[4]200$\n\t[5]400$\t[6]600$\n\t[7]800$\t[8]1000\t\n\t[9]Exit\n";
    cout << "===========================================\n";

    PerfromQuickWithdraw(ReadQuickWithdrawOption());
}
void ShowNormalWithdrawScreen()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tNormal Withdraw Screen\n";
    cout << "===========================================\n";

    double Amount = ReadWithdrawAmount("\nEnter An Amount Multiple Of 5's : ");

    if (Amount > CurrentClient.AccountBalance)
    {
        cout << "\nThe Amount Exceeds Your Balance, Make Another Choice ";
        cout << "\n\nPress Anykey to Continue....";
        system("pause>0");
        ShowNormalWithdrawScreen();
    }
    else
    {
        Withdraw(Amount);
    }

}
void ShowDepositScreen()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tDeposit Screen\n";
    cout << "===========================================\n";
    double Amount = ReadDepositAmount("Enter A Positive Deposit Amount : ");

    DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, Amount);
    CurrentClient.AccountBalance += Amount;
}
void ShowCheckBalanceScreen()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tCheck Balance Screen\n";
    cout << "===========================================\n";
    cout << "Your Balance Is : "
         << CurrentClient.AccountBalance << "$";
}

void PerfromMainMenueOption(enMainMenueOptions Option)
{
    switch (Option)
    {
        case enMainMenueOptions::eQuickWithdraw:
            ShowQuickWithdrawScreen();
            GoBackToMainMenue();
            break;
        case enMainMenueOptions::eNormalWithdraw:
            ShowNormalWithdrawScreen();
            GoBackToMainMenue();
            break;
        case enMainMenueOptions::eDeposit:
            ShowDepositScreen();
            GoBackToMainMenue();
            break;
        case enMainMenueOptions::eCheckBalance:
            ShowCheckBalanceScreen();
            GoBackToMainMenue();
            break;
        case enMainMenueOptions::eLogout:
            Login();
            break;
        
    }
}
void ShowMainMenue()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tATM Main Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Quick Withdraw.\n";
    cout << "\t[2] Normal Withdraw.\n";
    cout << "\t[3] Deposit.\n";
    cout << "\t[4] Check Balance.\n";
    cout << "\t[5] Logout.\n";
    cout << "===========================================\n";
    PerfromMainMenueOption((enMainMenueOptions)ReadMainMenueOption());
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
            cout << "\nInvalid Account Number/Pin Code :( !!.";

        string AccountNumber = ReadClientAccountNumber("\nEnter Account Number : ");
        string PinCode = ReadClientPinCode("\nEnter Pin Code : ");

        LoginFaild = !LoadClientInfo(AccountNumber, PinCode);
    } while (LoginFaild);

    ShowMainMenue();   
}
void GoBackToMainMenue()
{
    cout << "\n\nPress any key to go back to Main Menue...";
    system("pause>0");
    ShowMainMenue();
}
int main()
{
    Login();
    return 0;
}