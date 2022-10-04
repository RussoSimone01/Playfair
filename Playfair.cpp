#include <iostream>
#include <string>
#include <stdlib.h>
#define DIM 5
using namespace std;

void tableSetup(char t[DIM][DIM], string key);
string textSetup(string text, bool testo);
string cifratura(string txt, char t[DIM][DIM], int cifra);
void locate(char t[DIM][DIM], char c, int res[2]);

int main()
{
    string text, key;
    char table[DIM][DIM];

    int continua;
    do
    {
        system("cls");
        cout << "\tPlayfair\n";
        cout << "1. Cifra\n";
        cout << "2. Decifra\n";
        cout << "0. Esci\n\n";
        cin >> continua;
        system("cls");

        if (continua == 1 || continua == 2)
        {
            cout << "Inserisci il testo da " << (continua == 1 ? "" : "de") << "cifrare: ";
            cin.ignore();
            getline(cin, text);
            text = textSetup(text, continua == 1);

            cout << "Inserire la chiave: ";
            getline(cin, key);
            key = textSetup(key, false);
            tableSetup(table, key);

            string res = cifratura(text, table, continua);
            cout << endl
                 << (continua == 1 ? "C" : "Dec") << "ifratura: " << res << "\n\n";
            system("pause");
        }

    } while (continua);
    return (0);
}

void tableSetup(char t[DIM][DIM], string key)
{
    char alfabeto[25];
    int noJ = 0;                              // la j non è considerata, viene accorpata alla i
    for (int i = 0; i < (DIM * DIM) + 1; i++) // inizializzo l'array con tutte le lettere tranne la j
        if (i != 'J' - 'A')
            alfabeto[i - noJ] = i + 'A';
        else
            noJ = 1;

    bool fineChiave = false;
    int k = 0;
    for (int i = 0; i < DIM; i++)
    {
        for (int j = 0; j < DIM; j++)
        {
            if (fineChiave)
            {
                while (!alfabeto[k] && k < (DIM * DIM))
                    k++;
                t[i][j] = alfabeto[k++];
            }
            else
            {
                while (!alfabeto[key[k] - 'A' - (key[k] > 'I' ? 1 : 0)] && key[k]) // scorro finché non trovo una lettera della chiave non ancora utilizzata
                    k++;
                if (key[k]) // se la trovo
                {
                    alfabeto[key[k] - 'A' - (key[k] > 'I' ? 1 : 0)] = 0;
                    t[i][j] = key[k];
                }
                else // se la chiave è stata scorsa completamente
                {
                    fineChiave = true;
                    k = 0;
                    j--;
                }
            }
        }
    }
}

string textSetup(string text, bool testo)
{
    string res(text.length(), 0);
    int j = 0;
    for (int i = 0; text[i]; i++)
    {
        if ((text[i] >= 'A' && text[i] <= 'Z') || (text[i] >= 'a' && text[i] <= 'z'))
        {
            if (text[i] == 'J' || text[i] == 'j')
                res[j] = 'I';
            else
                res[j] = toupper(text[i]);

            if (testo && (j + 1) % 2 == 0 && res[j] == res[j - 1])
            {
                res[j + 1] = res[j];
                res[j] = 'X';
                j++;
            }
            j++;
        }
    }

    if (testo && j % 2 != 0)
    {
        res.resize(j + 2, 0);
        res[j] = 'X';
    }
    else
        res.resize(j + 1);

    return res;
}

string cifratura(string txt, char t[DIM][DIM], int cifra)
{
    string res(txt.length(), 0);
    int shift = cifra == 1 ? 1 : -1;
    int pos[2][2];
    for (int i = 0; txt[i]; i += 2)
    {
        locate(t, txt[i], pos[0]);
        locate(t, txt[i + 1], pos[1]);

        if (pos[0][0] == pos[1][0]) // se stessa riga, scambiati con lettera a destra
        {
            res[i] = t[pos[0][0]][(pos[0][1] + shift + DIM) % DIM];
            res[i + 1] = t[pos[1][0]][(pos[1][1] + shift + DIM) % DIM];
        }
        else if (pos[0][1] == pos[1][1]) // se stessa colonna, scambiati con lettera sottostante
        {
            res[i] = t[(pos[0][0] + shift + DIM) % DIM][pos[0][1]];
            res[i + 1] = t[(pos[1][0] + shift + DIM) % DIM][pos[1][1]];
        }
        else // altrimenti, scambiati con lettera stessa riga, ma colonna dell'altra lettera
        {
            res[i] = t[pos[0][0]][pos[1][1]];
            res[i + 1] = t[pos[1][0]][pos[0][1]];
        }
    }
    return res;
}

void locate(char t[DIM][DIM], char c, int pos[2])
{
    for (int i = 0; i < DIM; i++)
    {
        for (int j = 0; j < DIM; j++)
        {
            if (t[i][j] == c)
            {
                pos[0] = i;
                pos[1] = j;
                return;
            }
        }
    }
}