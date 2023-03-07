#include <iostream>
#include <string>
#include <conio.h>
#include <fstream>

using namespace std;

string AddGap(string n)
{
    string temp;
    string temp2;
    int size = n.length();
    temp += n[0];

    for (int i = 1; i < size; i++)
    {
        if (n[i] >= 'A' && n[i] <= 'Z')
        {
            temp += " ";
        }
        temp += n[i];
    }
    return temp;
}

string RemoveGap(string s)
{
    int n = s.length();
    int size = 0;

    for (int i = 0; i < n; i++) {
        if (s[i] == ' ')
        {
            s[i + 1] = toupper(s[i + 1]);
            continue;
        }
        else
        {
            s[size++] = s[i];
        }
    }
    return s.substr(0, size);
}

class Dictionary
{
public:
    Dictionary* left;
    Dictionary* right;

    int height;

    Dictionary()
    {
        word = "";
        meaning = "";
        left = NULL;
        right = NULL;
        height = 1;
    }

    Dictionary(string a, string b)
    {
        word = a;
        meaning = b;
        left = NULL;
        right = NULL;
        height = 1;
    }

    void setWord(string a, string b)
    {
        word = a;
        meaning = AddGap(b);
    }

    ~Dictionary()
    {
    }

    void Insert()
    {
        cout << "*******************************************" << endl;
        cout << "*************** ADD WORD ***************" << endl;
        cout << "Enter Word : "; getline(cin, word);
        cin.ignore();
        cout << "Enter Meaning: "; getline(cin, meaning);
    }

    void Display()
    {

        cout << " -- " << word << endl;
        cout << "\t " << meaning << endl;
    }

    void SetData(Dictionary S)
    {
        word = S.word;
        meaning = S.meaning;
    }

    string getWord()
    {
        return word;
    }

    void InsertIntoFile()
    {
        string b = RemoveGap(meaning);
        fstream f;
        f.open("sample.txt", ios::out | ios::app | ios::ate);
        if (f.is_open())
        {
            f << word << endl;
            f << b << endl;
        }
        f.close();
    }

private:
    string  word;
    string  meaning;
};

class AVL {
public:
    AVL()
    {
        root = NULL;
    }

    int max(int a, int b)
    {
        return (a > b) ? a : b;
    }

    int height(Dictionary* N)
    {
        if (N == NULL) {
            return 0;
        }
        return N->height;
    }

    Dictionary* newNode(Dictionary key)
    {
        Dictionary* node = new Dictionary;
        node->SetData(key);
        return (node);
    }

    Dictionary* rightRotate(Dictionary* y)
    {
        Dictionary* x = y->left;
        Dictionary* T2 = x->right;

        x->right = y;
        y->left = T2;

        y->height = max(height(y->left), height(y->right)) + 1;
        x->height = max(height(x->left), height(x->right)) + 1;

        return x;
    }

    Dictionary* leftRotate(Dictionary* x) {
        Dictionary* y = x->right;
        Dictionary* T2 = y->left;

        y->left = x;
        x->right = T2;

        x->height = max(height(x->left), height(x->right)) + 1;
        y->height = max(height(y->left), height(y->right)) + 1;
        return y;
    }

    int getBalanceFactor(Dictionary* N) {
        if (N == NULL)
        {
            return 0;
        }
        return height(N->left) - height(N->right);
    }

    Dictionary* insertNode(Dictionary* node, Dictionary key)
    {
        if (node == NULL) {
            return (newNode(key));
        }
        if (key.getWord() < node->getWord())
        {
            node->left = insertNode(node->left, key);
        }
        else if (key.getWord() > node->getWord())
        {
            node->right = insertNode(node->right, key);
        }
        else {
            return node;
        }

        node->height = 1 + max(height(node->left), height(node->right));
        int balanceFactor = getBalanceFactor(node);
        if (balanceFactor > 1) {
            if (key.getWord() < node->left->getWord()) {
                return rightRotate(node);
            }
            else if (key.getWord() > node->left->getWord())
            {
                node->left = leftRotate(node->left);
                return rightRotate(node);
            }
        }
        if (balanceFactor < -1) {
            if (key.getWord() > node->right->getWord()) {
                return leftRotate(node);
            }
            else if (key.getWord() < node->right->getWord()) {
                node->right = rightRotate(node->right);
                return leftRotate(node);
            }
        }
        return node;
    }

    Dictionary* nodeWithMimumValue(Dictionary* node)
    {
        Dictionary* current = node;
        while (current->left != NULL) {
            current = current->left;
        }
        return current;
    }

    Dictionary* deleteNode(Dictionary* root, string key)
    {
        if (root == NULL)
        {
            return root;
        }
        if (key < root->getWord())
        {
            root->left = deleteNode(root->left, key);
        }
        else if (key > root->getWord())
        {
            root->right = deleteNode(root->right, key);
        }
        else {
            if ((root->left == NULL) || (root->right == NULL))
            {
                Dictionary* temp = root->left ? root->left : root->right;
                if (temp == NULL) {
                    temp = root;
                    root = NULL;
                }
                else
                    *root = *temp;
                free(temp);
            }
            else
            {
                Dictionary* temp = nodeWithMimumValue(root->right);
                root->SetData(*temp);
                root->right = deleteNode(root->right, temp->getWord());
            }
        }

        if (root == NULL) {
            return root;
        }

        root->height = 1 + max(height(root->left), height(root->right));

        int balanceFactor = getBalanceFactor(root);

        if (balanceFactor > 1) {
            if (getBalanceFactor(root->left) >= 0)
            {
                return rightRotate(root);
            }
            else
            {
                root->left = leftRotate(root->left);
                return rightRotate(root);
            }
        }
        if (balanceFactor < -1) {
            if (getBalanceFactor(root->right) <= 0) {
                return leftRotate(root);
            }
            else {
                root->right = rightRotate(root->right);
                return leftRotate(root);
            }
        }
        return root;
    }

    Dictionary* getroot()
    {
        return root;
    }

    void setroot(Dictionary* p)
    {
        root = p;
    }

    void preorder(Dictionary* p)
    {
        if (p == NULL) {
            return;
        }
        p->Display();
        cout << endl;
        preorder(p->left);
        preorder(p->right);
    }

    void inorder(Dictionary* p)
    {
        if (p == NULL) {
            return;
        }
        p->Display();
        cout << endl;
        inorder(p->left);
        inorder(p->right);
    }

    void ResetFile(Dictionary* p)
    {
        if (p == NULL) {
            return;
        }
        ResetFile(p->left);
        p->InsertIntoFile();
        ResetFile(p->right);
    }

    void Reset()
    {
        fstream f;
        f.open("sample.txt", ios::out | ios::trunc);
        f.close();

        ResetFile(root);
    }

    void postorder(Dictionary* p)
    {
        if (p == NULL) {
            return;
        }
        p->Display();
        cout << endl;
        postorder(p->left);
        postorder(p->right);
    }

    void GetDataFromFile()
    {
        string a;
        string b;
        Dictionary temp;
        ifstream f;
        f.open("sample.txt", ios::in);
        if (f.is_open())
        {
            while (f.good())
            {
                f >> a;
                f >> b;
                temp.setWord(a, b);
                root = insertNode(getroot(), temp);
            }
        }
        f.close();
    }

    bool isEmpty()
    {
        if (root == NULL)
        {
            return true;
        }
        return false;
    }

    Dictionary* Find(string value)
    {
        Dictionary* p = root;
        if (isEmpty())
        {
            return p;
        }
        else
        {
            while (p != NULL)
            {
                if (p->getWord() == value) {
                    return p;
                }
                else if (value < p->getWord())
                {
                    p = p->left;
                }
                else if (value > p->getWord())
                {
                    p = p->right;
                }
            }
            return p;
        }
    }
private:
    Dictionary* root;
};

void Admin_Menu(AVL& avl)
{
    char choice;
    Dictionary temp;
    Dictionary* temp2;
    string num;
    string word;
    bool end = false;

    string password;
    system("cls");
    cout << " ******************************************************" << endl;
    cout << " ****************** ENTER PASSWORD ********************" << endl;
    cout << endl;
    cout << "\t\tPassword: ";

    cin >> password;
    cin.ignore();

    if (password == "admin123") {
        while (!end)
        {
            system("cls");
            cout << " ****************************************" << endl;
            cout << " Press 1. Insert." << endl;
            cout << " Press 2. Delete." << endl;
            cout << " Press 3. Search." << endl;
            cout << " Press 4. Display All." << endl;
            cout << " Press 5. Exit" << endl << endl;
            cout << " ****************************************" << endl;
            choice = _getch();
            switch (choice)
            {
            case '1':
                system("cls");
                temp.Insert();
                avl.setroot(avl.insertNode(avl.getroot(), temp));
                avl.Reset();
                break;

            case '2':
                system("cls");
                cout << " *******************************************" << endl;
                cout << " *************** DELETE WORD ***************" << endl;
                cout << endl;
                cout << " Word : "; cin >> num;
                avl.setroot(avl.deleteNode(avl.getroot(), num));
                avl.Reset();
                break;
            case '3':
                system("cls");
                cout << "*******************************************" << endl;
                cout << "*************** SEARCH WORD ***************" << endl;
                cout << "Word : "; cin >> word;
                temp2 = avl.Find(word);
                cout << endl;
                if (temp2 != nullptr)
                {
                    temp2->Display();
                }
                else
                {

                    cout << endl;
                    cout << "            WORD NOT FOUND               " << endl;
                }
                (void)_getch();
                break;
            case '4':
                system("cls");
                cout << " ****************************************" << endl;
                cout << " *************** ALL WORDS **************" << endl;
                avl.inorder(avl.getroot());
                (void)_getch();
                break;
            case '5':
                end = true;
                break;
            default:
                break;
            }
        }
    }
    else
    {
        cout << " ******************************************************" << endl;
        cout << " **************** PASSWORD NOT FOUND ******************" << endl;
    }

}

void User_Menu(AVL& avl)
{
    char choice;
    Dictionary* temp2;
    string word;
    bool end = false;
    while (!end)
    {
        system("cls");
        cout << " ****************************************" << endl;
        cout << " Press 1. Search." << endl;
        cout << " Press 2. Display All." << endl;
        cout << " Press 3. Exit" << endl << endl;
        cout << " ****************************************" << endl;
        choice = _getch();
        switch (choice)
        {
        case '1':
            system("cls");
            cout << " *******************************************" << endl;
            cout << " *************** SEARCH WORD ***************" << endl;
            cout << " Word : "; cin >> word;
            temp2 = avl.Find(word);
            cout << endl;
            if (temp2 != nullptr)
            {
                temp2->Display();
            }
            else
            {

                cout << endl;
                cout << "            WORD NOT FOUND               " << endl;
            }
            (void)_getch();
            break;
        case '2':
            system("cls");
            cout << " ****************************************" << endl;
            cout << " *************** ALL WORDS **************" << endl;
            avl.inorder(avl.getroot());
            (void)_getch();
            break;
        case '3':
            end = true;
            break;
        default:
            break;
        }
    }
}

void main()
{
    AVL avl;
    avl.GetDataFromFile();
    char choice;
    Dictionary temp;
    Dictionary* temp2;
    string num;
    string word;
    bool end = false;
    while (!end)
    {
        system("cls");
        cout << " ****************************************" << endl;
        cout << " Welcome to an English to English Dictionary created by: " << endl << endl;
        cout << " Reg# \t\t Names" << endl;
        cout << " 1912194 \t Umar Ahmad Khan" << endl;
        cout << " 1912191 \t Sudhakar Kumar" << endl;
        cout << " 1912181 \t Fajjar Shahzad" << endl;
        cout << " 1912168 \t Nouman Ali Shah" << endl;
        cout << " 1912160 \t Rafay Sarwar" << endl << endl;

        cout << " ****************MAIN MENU***************" << endl << endl;
        cout << " Enter 1. Login As Admin" << endl;
        cout << " Enter 2. Searh/Display " << endl << endl;
        cout << " ****************************************" << endl;

        choice = _getch();
        switch (choice)
        {
        case '1':
            system("cls");
            Admin_Menu(avl);
            break;

        case '2':
            system("cls");
            User_Menu(avl);
            break;
        case '3':
            end = true;
            break;
        default:
            break;
        }
    }
}






