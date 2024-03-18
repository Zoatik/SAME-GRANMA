#include <iostream>
#include <string>
#include <vector>

/**************************
auteur         : Axel Hall
nom du fichier : 346228.cpp
version        : 8.1
***************************/


using namespace std;


///constantes error.txt
const string NOT_IN_CAPITAL_LETTERS("The word is not purely in capital letters");
const string DOUBLE_WORD("Each word can be present only once");
const string EMPTY_DICO("The dictionnary cannot be empty");
const string NO_ANAGRAM("There is no anagram for this message and this dictionary");

struct Mot_struct;
typedef vector<Mot_struct> Vect_mots_struct;


///dico et message
const void afficher_liste_mot_struct(const Vect_mots_struct& liste);
Vect_mots_struct tri_dico(const Vect_mots_struct& liste_mot);
Vect_mots_struct recup_liste_mots(bool is_dico);//true->recup dico,
                                                //false->recup anagrammes
void set_list_messages(vector<Vect_mots_struct> &liste_alpha_m_vect);

///anagrammes
void display_anag(const Vect_mots_struct &dico_trie,
                  const vector<Vect_mots_struct> &liste_alpha_m_vect);
bool rech_anagramme(const Vect_mots_struct &dico, const string& alpha_m,
                    const string& anagramme, string& anag_fin);
string message_contient_mot(const string &mot_contenu, const string &message);
            //retourne le message réduit ou "." si le mot n'est pas contenu
Vect_mots_struct enlever_mot(const Vect_mots_struct &dico, size_t pos);

///fonctions get_ les différentes propriétés d'un mot(string)
int get_nbT(const string& mot);
int get_nbD(const string& mot);
string get_alpha(const string& mot);

///conversions liste, liste de liste
vector<Vect_mots_struct> vector_to_list_of_vector(const Vect_mots_struct& liste_triee,
                                                  bool is_nbD);//pour le tri nbT

vector<Vect_mots_struct> tri_nbD(const vector<Vect_mots_struct>& liste);
Vect_mots_struct tri_alpha(const Vect_mots_struct& liste_mot, bool alpha);
Vect_mots_struct fusion_alpha(const Vect_mots_struct& left,
                         const Vect_mots_struct& right, bool alpha);
Vect_mots_struct tri_alphabetique(const Vect_mots_struct& liste_mot);
int compare(const Mot_struct &mot_1, const Mot_struct &mot_2, bool alpha);
            //si alpha true, en fonction de m_alpha. Sinon m_mot
            //return 1 si le premier mot a une valeur alpha plus petite que le second
            //return 2 si le deuxième mot...
            //return 3 si les deux mots ont la meme valeur alpha

///tri lettres d'un mot (a-z) et enlève ou non les doublons (nbT, nbD)
string fusion_listes_char(const string& left, const string& right, bool supp_doublons);
string tri_fusion_char(const string& mot, bool supp_doublons);

///tri en fonction de la taille des mots(nbT ou nbD)
Vect_mots_struct fusion_listes_mots_taille(const Vect_mots_struct left,
                                      const Vect_mots_struct right, bool is_nbD);
Vect_mots_struct tri_fusion_mots_taille(const Vect_mots_struct liste_mots,
                                        bool is_nbD);

///structure d'un mot avec ses propriétés
struct Mot_struct {
    //constructeur
    Mot_struct(string mot) :
        m_mot(mot), m_nbT(get_nbT(mot)), m_nbD(get_nbD(mot)), m_alpha(get_alpha(mot)){}

    //champs
	string m_mot;
	int m_nbT;
	int m_nbD;
	string m_alpha;
};

/*************************************************************************************/
/*********************************DEBUT PROGRAMME*************************************/
/*************************************************************************************/
int main()
{
    ios::sync_with_stdio(false);

    Vect_mots_struct dico(recup_liste_mots(true));
    Vect_mots_struct dico_trie = tri_dico(dico);
    afficher_liste_mot_struct(dico_trie);
    string dico_alpha;
    for(size_t i(0); i<dico_trie.size(); i++)
    {
        dico_alpha+=(dico_trie[i].m_alpha);
    }
    dico_alpha = get_alpha(dico_alpha);

    vector<Vect_mots_struct> liste_alpha_m_vect;
    set_list_messages(liste_alpha_m_vect);

    display_anag(dico_trie, liste_alpha_m_vect);

    return 0;
}
/*************************************************************************************/

const void afficher_liste_mot_struct(const Vect_mots_struct& liste)
{
    size_t t_liste(liste.size());
    for (size_t i = 0; i<t_liste; i++)
    {
        cout<<liste[i].m_mot<<"\n";
    }
}
/*************************************************************************************/

Vect_mots_struct tri_dico(const Vect_mots_struct& liste_mot)
{
    Vect_mots_struct final_liste;
    Vect_mots_struct temp = tri_fusion_mots_taille(liste_mot, false);
    vector<Vect_mots_struct> liste_tri_nbT = vector_to_list_of_vector(temp, false);
    vector<Vect_mots_struct> liste_tri_nbD = tri_nbD(liste_tri_nbT);
    vector<Vect_mots_struct> liste_tri_alpha;

    size_t t_liste_tri_nbD(liste_tri_nbD.size());
    liste_tri_nbD.reserve(t_liste_tri_nbD);//on réserve la place à allouer au vecteur

    for (size_t j = 0; j<t_liste_tri_nbD; j++)
    {
        liste_tri_alpha.push_back(tri_alpha(liste_tri_nbD[j], true));
    }

    size_t t_liste_tri_alpha(liste_tri_alpha.size());
    liste_tri_alpha.reserve(t_liste_tri_alpha);

    for (size_t l = 0; l<t_liste_tri_alpha; l++)
    {
        size_t t_mot_tri_alpha(liste_tri_alpha[l].size());
        final_liste.reserve(t_mot_tri_alpha);

        for (size_t m = 0; m<t_mot_tri_alpha; m++)
        {
            final_liste.push_back(liste_tri_alpha[l][m]);
        }
    }
    return final_liste;
}
/*************************************************************************************/

Vect_mots_struct recup_liste_mots(bool is_dico)
{
    Vect_mots_struct liste;
    string mot;

    do{
        cin>>mot;
        Mot_struct mot_s(mot);
        if(is_dico)
        {
            if(mot_s.m_alpha[mot_s.m_nbD-1]>90 || mot_s.m_alpha[0]<65)//trouve minusc.
            {
                if (mot_s.m_nbT != 1 || mot_s.m_alpha[0] != '.')
                {
                    cout<<NOT_IN_CAPITAL_LETTERS<<"\n";
                    exit(0);
                }
            }
        }
        liste.push_back(mot_s);
    }while(mot != "." && (mot != "*" || is_dico));//prend tous les éléments du buffer

    if (mot != "*")
    {
        if (liste[0].m_mot == ".")
        {
            cout<<EMPTY_DICO<<endl;
            exit(0);
        }
        if (liste[liste.size()-1].m_mot.size()==1)
        {
            liste.pop_back();//enlève le point
        }
    }
    return liste;
}
/*************************************************************************************/

void set_list_messages(vector<Vect_mots_struct> &liste_alpha_m_vect)
{
    while (true)
    {
        Vect_mots_struct alpha_m_vect(recup_liste_mots(false));
        if(alpha_m_vect[alpha_m_vect.size()-1].m_mot == "*")
        {
            if (alpha_m_vect.size() > 1)
            {
                alpha_m_vect.pop_back();
                liste_alpha_m_vect.push_back(alpha_m_vect);
            }
            break;
        }
        else
        {
            liste_alpha_m_vect.push_back(alpha_m_vect);
        }
    }
}
/*************************************************************************************/

void display_anag(const Vect_mots_struct &dico_trie,
                  const vector<Vect_mots_struct> &liste_alpha_m_vect)
{
    size_t taille_alpha_m_liste_vect = liste_alpha_m_vect.size();
    for (size_t i = 0; i< taille_alpha_m_liste_vect; i++)
    {
        if (i == 0)
            cout<<"\n";
        string alpha_m;
        bool is_in_capital_letters(true);
        size_t taille_alpha_m_liste = liste_alpha_m_vect[i].size();
        for (size_t j = 0; j<taille_alpha_m_liste; j++)
        {
            int pos_fin = liste_alpha_m_vect[i][j].m_alpha.size()-1;
            if(liste_alpha_m_vect[i][j].m_alpha[pos_fin] > 90 ||
               liste_alpha_m_vect[i][j].m_alpha[0] < 65)//test mot en majuscule
            {
                is_in_capital_letters = false;
                cout<<NOT_IN_CAPITAL_LETTERS<<"\n";
            }
            alpha_m.append(liste_alpha_m_vect[i][j].m_mot);
        }
        alpha_m = tri_fusion_char(alpha_m, false);
        string anag_fin = "";
        bool success = rech_anagramme(dico_trie, alpha_m,"", anag_fin);
        if (!success && is_in_capital_letters==true)
        {
            cout<<NO_ANAGRAM<<"\n";
        }
        else
            cout<<anag_fin;

    if(i<taille_alpha_m_liste_vect-1)
        cout<<"\n";
    }
}
/*************************************************************************************/

bool rech_anagramme(const Vect_mots_struct &dico, const string& alpha_m,
                    const string& anagramme, string& anag_fin)
{
    size_t taille_dico(dico.size());
    if (taille_dico == 0)
    {
        return false;
    }
    bool success(false);

    for (size_t i = 0; i<taille_dico; i++)
    {
        string alpha_m_next = message_contient_mot(dico[i].m_alpha, alpha_m);
        if (alpha_m_next != ".")//point signigie que le mot n'est pas contenu
        {
            string anag_next(anagramme);
            anag_next.append(dico[i].m_mot+" ");

            if (alpha_m_next.size() == 0)
            {
                anag_next.pop_back();//enlève l'espace
                anag_next += "\n";
                anag_fin += anag_next;
                success = true;
            }
            else
            {
                Vect_mots_struct dico_next = enlever_mot(dico, i);
                if (rech_anagramme(dico_next, alpha_m_next, anag_next, anag_fin))
                    success = true;
            }
        }
    }
    return success;
}
/*************************************************************************************/

string message_contient_mot(const string &mot_contenu, const string &message)
{
    size_t i(0), t_message(message.size()), t_mot_contenu(mot_contenu.size());

    if(t_message<t_mot_contenu)
        return ".";

    string new_mot;
    string message_reduit;
    new_mot.reserve(t_mot_contenu);
    message_reduit.reserve(t_message-t_mot_contenu);

    while(mot_contenu[0] != message[i])
    {
        if(i == t_message-t_mot_contenu)
            return ".";
        message_reduit.push_back(message[i]);
        i++;
    }

    size_t j(0);
    while (j+i<t_message)
    {
        if (mot_contenu[j] == message[i+j])
        {
            new_mot.push_back(mot_contenu[j]);
            j++;
        }
        else
        {
            message_reduit.push_back(message[i+j]);
            i++;
        }
        if(new_mot == mot_contenu)
        {
            message_reduit.append(message.substr(i+j));
            return message_reduit;
        }
    }
    return ".";
}
/*************************************************************************************/

Vect_mots_struct enlever_mot(const Vect_mots_struct &dico, size_t pos)
{
    size_t t_dico(dico.size());
    Vect_mots_struct dico_reduit;
    dico_reduit.reserve(t_dico-1);

    for (size_t i = 0; i<t_dico; i++)
    {
        if (i != pos)
        {
            dico_reduit.push_back(dico[i]);
        }
    }
    return dico_reduit;
}
/*************************************************************************************/

int get_nbT(const string& mot)
{
    return mot.size();
}

int get_nbD(const string& mot)
{
    return tri_fusion_char(mot,true).size();
}
string get_alpha(const string& mot)
{
    return tri_fusion_char(mot,false);
}
/*************************************************************************************/

vector<Vect_mots_struct> vector_to_list_of_vector(const Vect_mots_struct& liste_triee,
                                                  bool is_nbD)//false pour le tri nbT
{
    size_t t_liste_triee(liste_triee.size());
    vector<Vect_mots_struct> liste_vect;
    Vect_mots_struct sub_vector;

    for (size_t i = 0; i<t_liste_triee-1; i++)
    {
        sub_vector.push_back(liste_triee[i]);
        if(is_nbD)
        {
            if (liste_triee[i].m_nbD != liste_triee[i+1].m_nbD)
            {
                liste_vect.push_back(sub_vector);
                sub_vector = {};
                sub_vector.shrink_to_fit();//libère l'espace mémoire
            }
        }
        else
        {
            if (liste_triee[i].m_nbT != liste_triee[i+1].m_nbT)
            {
                liste_vect.push_back(sub_vector);
                sub_vector = {};
                sub_vector.shrink_to_fit();
            }
        }
    }

    sub_vector.push_back(liste_triee[t_liste_triee-1]);
    liste_vect.push_back(sub_vector);
    return liste_vect;
}
/*************************************************************************************/

vector<Vect_mots_struct> tri_nbD(const vector<Vect_mots_struct>& liste)
{
    size_t t_liste(liste.size());
    vector<Vect_mots_struct> liste_liste_nbD;

    for (size_t i = 0; i<t_liste; i++)
    {
        Vect_mots_struct temp = tri_fusion_mots_taille(liste[i], true);
        vector<Vect_mots_struct> pass_liste = vector_to_list_of_vector(temp,true);
        size_t t_pass_liste(pass_liste.size());

        for (size_t j = 0; j<t_pass_liste; j++)
        {
            liste_liste_nbD.push_back(pass_liste[j]);
        }
    }

    return liste_liste_nbD;
}
/*************************************************************************************/

Vect_mots_struct tri_alpha(const Vect_mots_struct& liste_mot, bool alpha)
{// si alpha = true, on fait le tri selon m_alpha, sinon selon m_mot
    size_t t(liste_mot.size());

    if (t == 1)
    {
    return liste_mot;
    }

    else if(t == 2)
    {
        switch (compare(liste_mot[0], liste_mot[1], alpha))
        {
        case 1:
            return liste_mot;
        case 2:
            {
                return {liste_mot[1], liste_mot[0]};
            }
        case 3:
            if(alpha)
            {
                return tri_alphabetique(liste_mot);
            }
            else
            {
                cout<<DOUBLE_WORD<<"\n";//cas où 2 meme mots dans le dico
                exit(0);
            }
        default:
            break;
        }
    }
    int mid(t/2);
    Vect_mots_struct sub_vect_left = {liste_mot.begin(), liste_mot.begin() + mid+t%2};
    Vect_mots_struct sub_vect_right = {liste_mot.begin()+mid+t%2, liste_mot.end()};
    Vect_mots_struct left_s = tri_alpha(sub_vect_left, alpha);
    Vect_mots_struct right_s = tri_alpha(sub_vect_right, alpha);

    return fusion_alpha(left_s,right_s, alpha);
}
/*************************************************************************************/

Vect_mots_struct fusion_alpha(const Vect_mots_struct& left,
                              const Vect_mots_struct& right, bool alpha)
{
    size_t t_1(left.size()), t_2(right.size());
    size_t i(0), j(0);
    Vect_mots_struct liste_fusion;
    liste_fusion.reserve(t_1+t_2);
    while (i<t_1 && j<t_2)
    {
        if (compare(left[i],right[j], alpha) == 1)
            liste_fusion.push_back(left[i++]);

        else if (compare(left[i],right[j], alpha) == 2)
            liste_fusion.push_back(right[j++]);

        else
        {
            if (compare(left[i],right[j], false) == 1)
                liste_fusion.push_back(left[i++]);

            else if(compare(left[i],right[j], false) == 2)
                liste_fusion.push_back(right[j++]);

            else
            {
                cout<<DOUBLE_WORD<<"\n";
                exit(0);
            }
        }
    }
    while (i<t_1)
       liste_fusion.push_back(left[i++]);

    while (j<t_2)
       liste_fusion.push_back(right[j++]);

    return liste_fusion;
}
/*************************************************************************************/

Vect_mots_struct tri_alphabetique(const Vect_mots_struct& liste_mot)
{
    return tri_alpha(liste_mot, false);
}
/*************************************************************************************/

int compare(const Mot_struct &mot_1, const Mot_struct &mot_2, bool alpha)
{
   size_t t_mot_1(mot_1.m_mot.size());

    if (alpha)
    {
        for (size_t i = 0; i<t_mot_1; i++)//on sait que les deux mots ont la meme taille
        {
            if(mot_1.m_alpha[i] < mot_2.m_alpha[i])
                return 1;

            else if(mot_1.m_alpha[i] > mot_2.m_alpha[i])
                return 2;
        }
    }

    else
    {
       for (size_t i = 0; i<t_mot_1; i++)
        {
            if(mot_1.m_mot[i] < mot_2.m_mot[i])
                return 1;

            else if(mot_1.m_mot[i] > mot_2.m_mot[i])
                return 2;
        }
    }

    return 3;
}
/*************************************************************************************/

string fusion_listes_char(const string& left, const string& right, bool supp_doublons)
{
    size_t t_1(left.size()), t_2(right.size());
    size_t i(0), j(0);
    string liste_fusion;
    while (i<t_1 && j<t_2)
    {
        if (left[i] < right[j])
            liste_fusion.push_back(left[i++]);

        else if(left[i] == right[j] && supp_doublons == true)
        {//enlève la deuxième partie des doublons
            liste_fusion.push_back(left[i++]);
            j++;
        }
        else
            liste_fusion.push_back(right[j++]);
    }
    while (i<t_1)
       liste_fusion.push_back(left[i++]);

    while (j<t_2)
       liste_fusion.push_back(right[j++]);

    return liste_fusion;
}
/*************************************************************************************/

string tri_fusion_char(const string& mot, bool supp_doublons)
{
    int t = mot.size();

    if (t == 0 || t == 1)
        return mot;

    if (t == 2)
    {
        if (mot[0]>mot[1])
            return {mot[1], mot[0]};

        else if (mot[0] == mot[1] && supp_doublons == true)
            return {mot[0]};    //permet d'enlever la première partie des doublons

        return mot;
    }

    int mid = t/2;
    string sub_string_left = mot.substr(0,mid+t%2);
    string sub_string_right = mot.substr(mid+t%2);
    string left_s = tri_fusion_char(sub_string_left, supp_doublons);
    string right_s = tri_fusion_char(sub_string_right, supp_doublons);

    return fusion_listes_char(left_s,right_s, supp_doublons);
}
/*************************************************************************************/

Vect_mots_struct fusion_listes_mots_taille(const Vect_mots_struct left,
                                           const Vect_mots_struct right, bool is_nbD)
{
    size_t t_1(left.size()), t_2(right.size());
    size_t i(0), j(0);
    Vect_mots_struct liste_fusion;
    liste_fusion.reserve(t_1+t_2);

    while (i<t_1 && j<t_2)
    {
        if(is_nbD)
        {
            if (left[i].m_nbD < right[j].m_nbD)
                liste_fusion.push_back(left[i++]);

            else
                liste_fusion.push_back(right[j++]);
        }
        else
        {
            if (left[i].m_nbT < right[j].m_nbT)
                liste_fusion.push_back(left[i++]);

            else
                liste_fusion.push_back(right[j++]);
        }
    }

    while (i<t_1)
       liste_fusion.push_back(left[i++]);

    while (j<t_2)
       liste_fusion.push_back(right[j++]);

    return liste_fusion;
}
/*************************************************************************************/

Vect_mots_struct tri_fusion_mots_taille(const Vect_mots_struct liste_mots, bool is_nbD)
{
    int t = liste_mots.size();
    if (t == 0 || t == 1)
        return liste_mots;

    if(is_nbD)
    {
        if (t == 2)
        {
            if (liste_mots[0].m_nbD>liste_mots[1].m_nbD)
                return {liste_mots[1], liste_mots[0]};

            return liste_mots;
        }
    }
    else
    {
        if (t == 2)
        {
            if (liste_mots[0].m_nbT>liste_mots[1].m_nbT)
                return {liste_mots[1], liste_mots[0]};

            return liste_mots;
        }
    }

    int mid = t/2;
    Vect_mots_struct sub_vect_left = {liste_mots.begin(), liste_mots.begin()+mid+t%2};
    Vect_mots_struct sub_vect_right = {liste_mots.begin()+mid+t%2, liste_mots.end()};
    Vect_mots_struct left_s = tri_fusion_mots_taille(sub_vect_left, is_nbD);
    Vect_mots_struct right_s = tri_fusion_mots_taille(sub_vect_right, is_nbD);

    return fusion_listes_mots_taille(left_s,right_s, is_nbD);
}
/*************************************************************************************/






