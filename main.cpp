#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
using namespace std;


class Utilizator{
    static string nume;
    static string prenume;
    static string dataNasterii;
    static string parola;
    static Utilizator *instance;
    Utilizator() {}
public:
    static Utilizator* getInstance(){
        if(instance == nullptr){
            instance = new Utilizator();
        }
        return instance;
    }

    void setNume(const string& n) { nume = n; }
    string getNume() const { return nume; }

    void setPrenume(const string& pr) { prenume = pr;}
    string getPrenume() const { return prenume;}

    void setDataNasterii(const string& dn) { dataNasterii = dn; }
    string getDataNasterii() const { return dataNasterii; }

    void setParola(const string& p) { parola = p; }
    string getParola() const { return parola; }
};

Utilizator* Utilizator::instance = nullptr;
string Utilizator::nume = "";
string Utilizator::prenume="";
string Utilizator::dataNasterii = "";
string Utilizator::parola = "";

class Cont {
protected:
    double suma;
public:
    Cont(double suma_initiala = 0) : suma(suma_initiala) {}

    virtual void Dobanda()=0;
    virtual string afisareTip() const = 0;

    virtual void retrageBani(double suma) {
        if (suma <= this->suma) {
            this->suma -= suma;
            cout << "S-au retras " << suma << " lei. Sold actual: " << this->suma << " lei."<<endl;
        } else {
            cout << "Fonduri insuficiente."<<endl;
        }
    }

    void depuneBani(double suma) {
        this->suma += suma;
        cout << "S-au depus " << suma << " lei. Sold actual: " << this->suma << " lei."<<endl;
    }

    double getSuma() const {
        return suma;
    }
    void setSuma( const double& suma){this->suma= suma ;}
    virtual ~Cont(){}

};


class ContEconomii : public Cont{
    double rataDobanda;
    int limitareTranzactii;
public:
    ContEconomii(const double& sumaInitiala=0,const double& rataDobdanda=0.01,const int& limitareTranzactii=10): Cont(sumaInitiala), rataDobanda(rataDobanda), limitareTranzactii(limitareTranzactii){}
    void Dobanda() override {
        suma += suma * rataDobanda;
        cout << "Dobanda adaugata. Sold actual: " << suma << " lei."<<endl;
    }

    string afisareTip() const override {

       return "Cont economii";
    }

    void setLimitareTranzactii(const int& limitareTranzcatii) {this->limitareTranzactii = limitareTranzactii;}
    int getLimitareTranzactii() const { return limitareTranzactii; }
    int decrementareTranzactii() {
        limitareTranzactii-=1;
        return limitareTranzactii;
    }
    ~ContEconomii() {}
};


class ContCurent: public Cont {
public:
    ContCurent(const double& sumaInitiala=0): Cont(sumaInitiala) {}

    void Dobanda() override {
        cout<<"Conturile curente nu primesc dobanda!"<<endl;
    }
    string afisareTip() const override {
        return "Cont curent";
    }

    ~ContCurent(){}

};


class ContDepozit : public Cont {
    double rataDobanda;
public:
    ContDepozit(const double& sumaInitiala=0, const double& dobanda=0.03): Cont(sumaInitiala), rataDobanda(rataDobanda) {}

    void Dobanda() override {
        suma += suma * rataDobanda;
        cout << "Dobanda adaugata. Sold actual: " << suma << " lei."<<endl;
    }

    void retrageBani(double suma) override {
        cout << "Retragerea de bani nu este permisa pentru contul de depozit."<<endl;
    }

    string afisareTip() const override {
       return "Cont depozit";
    }
    ~ContDepozit(){}
};


class Card {
    unique_ptr<Cont> cont;
    string banca;
    int nrCard;

public:
    Card(const string& b, const int& nr, unique_ptr<Cont> cont) : banca(b), nrCard(nr), cont(move(cont)) {}

//    double getSuma() const { cont->getSuma(); }
//    void setSuma(const double& suma) { cont->setSuma(suma); }
    string getBanca() const { return banca; }
    int getNrCard() const { return nrCard; }
    void setBanca(const string& b) { banca = b; }
    void setNrCard(const int& nr) { nrCard = nr; }
    Cont* getCont() const {
        return cont.get();
    }
    void afisareTipCont() { cont->afisareTip();}

    void adaugaBani(double suma) {
        cont->setSuma(cont->getSuma() + suma);
    }
};


class Aplicatie {
    Utilizator *utilizator;
    vector<unique_ptr<Card>> carduri;
public:
    Aplicatie() {
        utilizator = nullptr;
    }

    void creeazaUtilizator(const string& n ,const string& pr, const string& dn, const string& p) {
        if ( utilizator->getNume() == n && utilizator->getPrenume() == pr && utilizator->getDataNasterii() == dn && utilizator->getParola() == p) {
            throw runtime_error("Utilizator deja existent!");
        }
        if(utilizator == nullptr) {
            utilizator = Utilizator::getInstance();
            utilizator->setNume(n);
            utilizator->setPrenume(pr);
            utilizator->setDataNasterii(dn);
            utilizator->setParola(p);

            int ni, nr;
            string b;
            cout << "Cate carduri doriti sa adaugati in contul dvs?" << endl;
            cin >> ni;
            for (int i = 1; i <= ni; i++) {
                cout << "Crad " << i << endl;
                cout << "Introduceti banca cardului: ";
                cin >> b;
                cout << "Introduceti numarul cardului: ";
                cin >> nr;
                adaugaCard(b, nr);

            }
            int alg;
            cout << "Cont creat cu succes! Doriti sa va intoarceti la meniu pentru a putea intra in cont? (1) Da" << endl; cin>>alg;
        }
        else {
            cout<<"Pe aceasta aplicatia a fost deja inregistrat un cont!"<<endl;
            cout<<"Doriti sa va intoarceti la meniu? (1) Da"<<endl; int alg; cin>>alg;
        }
    }

    void adaugaCard(const string& banca, const int& nr){
        int tipCont;
        cout<<"Selectati tipul contului: (1) Cont curent   (2) Cont de aconomii (3) Cont depozit"<<endl; cin>>tipCont;
        unique_ptr<Cont> cont;
        switch (tipCont) {
            case 1:
                cont = make_unique<ContCurent>();
                break;
            case 2:
                cont = make_unique<ContEconomii>();
                break;
            case 3:
                cont = make_unique<ContDepozit>();
                break;
            default:
                cout << "Optiune invalida!" << endl;
                return;
        }

        carduri.push_back(make_unique<Card>(banca, nr, move(cont)));
    }


    void PrefataCont(){
        cout<<"Buna ziua "<<utilizator->getNume()<<" "<<utilizator->getPrenume()<<" ,bine ati revenit pe contul dumnevoastra!"<<endl;
        cout<<"(1) Vizualizare sold."<<endl;
        cout<<"(2) Efectuati o tranzactie."<<endl;
        cout<<"(3) Vizualizati carduri curente."<<endl;
        cout<<"(4) Adauga card."<<endl;
        cout<<"(5) Adauga bani."<<endl;
        cout<<"(6) Elimina card."<<endl;
        cout<<"(7) Deconectare cont."<<endl;
    }

    void VizualizareSold(){
        int cnt=0,c;
        cout<<"Conturile dvs. disponibile sunt:"<<endl;
        for( const auto &card: carduri ){
            cout<<"Card "<< card->getBanca() <<" cu numarul: "<<card->getNrCard()<<" de tipul: "<<(card->getCont())->afisareTip() <<endl;
            cnt++;
        }
        bool valid= false;
        while(valid==false){
            int cnt1=0, a;
            cout<<"Indicati nr. contului pe care doriti sa-l selectati.:"<<endl; cin>>c;
            for( const auto &card: carduri ) {
                cnt1++;
                if (c == card->getNrCard()) {
                    valid = true;
                    cout << "Soldul este: " << card->getCont()->getSuma() << " lei." << endl;
                    cout<<"Doriti sa va intoarceti la pagina principala? (1) Da"<<endl; cin>>a;

                }
                if(cnt==cnt1 && valid == false){
                    cout<< "Numarul cardului a fost introdus gresit!" << endl;
                }
            }

        }
}

    void VizualizareCarduri(){
        for( const auto &card: carduri ){
            cout<<"Card "<< card->getBanca() <<" de tipul: "<<card->getCont()->afisareTip() <<" cu numarul: "<<card->getNrCard()<<" are soldul: "<<card->getCont()->getSuma()<<endl;
        }
}

    void EliminareCont(){
        int nr;
        cout << "Care este numarul cardului pe care doriti sa-l eliminati:" << endl; cin >> nr;


        auto it = find_if(carduri.begin(), carduri.end(),
                          [nr](const unique_ptr<Card>& card) {
                              return card->getNrCard() == nr;
                          });

        if (it != carduri.end()) {
            // Ștergem cardul din vector
            carduri.erase(it);
            cout << "Cardul a fost eliminat!" << endl;
        } else {
            cout << "Cardul nu a fost gasit!" << endl;
        }
}

    void inrareCont(const string& n,const string& pr, const string& dn, const string& p){
        if( utilizator->getNume() == n && utilizator->getPrenume() == pr,  utilizator->getDataNasterii() == dn && utilizator->getParola() == p){
             PrefataCont();
             int alg, c;

             while(cin>>alg){
             switch(alg){
                 case 1:{
                     VizualizareSold();
                     PrefataCont();
                     break;

                 }

                 case 2:{
                    adaugaBani();
                    PrefataCont();
                    break;

                 }

                 case 3:{
                     int alg;
                    VizualizareCarduri();
                    cout<<"Doriti sa va intoarceti la pagina principala? (1) Da "<<endl; cin>>alg;
                     PrefataCont();
                     break;
                 }

                 case 4: {
                     string b;
                     int nr,alg;
                     cout<<"Introduceti numele banci:"<<endl; cin>>b;
                     cout<<"Introduceti numarul cardului:"<<endl; cin>>nr;
                     adaugaCard(b,nr);
                     cout<<"Cardul a fost adaugat cu succes!"<<endl;
                     cout<<"Doriti sa va intoarceti la meniul principal? (1) Da"<<endl; cin>>alg;
                     PrefataCont();
                     break;

                 }
                 case 5: {

                     int alg; double s; bool cnt = false;
                     cout<<"Introduceti numarul cardului pe care doriti sa adaugati bani?"<<endl; cin>>alg;
                     cout<<"Introduceti ce suma vreti sa adaugati (in lei):"<<endl; cin>>s;
                     for( auto& card : carduri ){
                         if(card->getNrCard() == alg){
                             card->adaugaBani(s);
                             cnt= true;
                             cout<<"Banii au fost adaugati cu succes! Cardul cu numarul "<<card->getNrCard()<<" are soldul de " <<card->getCont()->getSuma()<<"."<<endl;
                         }
                     }
                     if(!cnt){
                         cout<<"Cardul cu numarul "<<alg<<" nu a fost gasit."<<endl;
                     }

                     PrefataCont();
                     break;
                 }
                 case 6:{
                     int alg;
                     EliminareCont();
                     cout<<"Doriti sa va intoarceti la pagina principala? (1) Da"<<endl; cin>>alg;
                     PrefataCont();
                     break;
                 }
                 case 7:{
                     cout<<"Sunteti sigur ca doriti sa iesiti de pe contul dumenavoastra? (1) Da   (2) Nu"<<endl; int alg; cin>>alg;
                     if(alg==2){
                         cout<<"Va vom redirectiona inapoi la meniu. (1) Ok"<<endl; int alg; cin>>alg;
                         PrefataCont();
                         break;
                     }
                     else{
                         return;
                     }

                 }

                 }
             }

        }
        else {
            int alg;
            cout<<"Contul nu a fost gasit!  Doriti sa va intoarceti la meniu? (1) Da"<<endl; cin>>alg;

        }

    }

    bool VerificareCard(const int& nrC) {
        for (auto& card : carduri) {
            if (nrC == card->getNrCard()) {
                Cont* cont = card->getCont();

                if (dynamic_cast<ContDepozit*>(cont)) {
                    cout << "Nu se pot face tranzactii de pe un cont de depozit." << endl;
                    return false;
                }

                if (auto* contEconomii = dynamic_cast<ContEconomii*>(cont)) {
                    if (contEconomii->getLimitareTranzactii() <= 0) {
                        cout << "Nu mai aveti tranzactii disponibile pentru acest cont de economii." << endl;
                        return false;
                    } else {
                        contEconomii->decrementareTranzactii();
                        cout<<"Pe acest card de economii mai aveti disponibile din acest moment "<<contEconomii->getLimitareTranzactii()<<" tranzactii."<<endl;
                        return true;
                    }
                }

                if (dynamic_cast<ContCurent*>(cont)) {
                    return true;
                }
            }
        }

        return false;
    }
    bool VerificareCard2(const int& nrC){
        for( auto& card : carduri){
            if(card->getNrCard() == nrC){
                return true;
            }
        }
        return false;
    }

    void adaugaBani(){
    int alg;
    cout<<"Inainte de a continua doriti sa vizualizati soldurile cardurilor? (1) Da  (2) Nu "<<endl; cin>>alg;
    if(alg==1){
        int a,c, c1;
       VizualizareCarduri();
       cout<<"Doriti sa continuati?  (1) Da  (2) Nu"<<endl; cin>>a;
       if(a==2){
           return;
       }
       cout<<"De pe care carduri din cele existente in aplicatie doriti sa trimiteti bani? Introduceti numarul cardului:"<<endl; cin>>c;
        if(VerificareCard(c)  ){
            cout<<"Introduceti numarul cardului pe care banii sa fie adaugati:"<<endl; cin>>c1;
            if(VerificareCard2(c1)){
                double s;
                cout<<"Introduceti suma pe care doriti sa o adaugati:"<<endl; cin>>s;

                for( auto &card : carduri){

                    if(c == card->getNrCard() && card->getCont()->getSuma() < s){
                        cout<<"Sold insuficient in cardul cu numarul "<<card->getNrCard()<<"."<<endl;
                        cout<<"Puteti adauga maxim "<<card->getCont()->getSuma()<<"lei."<<endl;
                    }
                    else if(c == card->getNrCard() ){
                        double sumaNoua= card->getCont()->getSuma() - s;
                        card->getCont()->setSuma(sumaNoua);
                        for(auto &card : carduri){
                            if(card->getNrCard() == c1){
                                double sumaN= card->getCont()->getSuma() + s;
                                card->getCont()->setSuma(sumaN);
                            }
                        }
                        cout<<" Suma a fost adaugata cu succes. Situatia financiara actuala a coturilor dvs. este:"<<endl;
                        VizualizareCarduri();
                        cout<<"Doriti sa va intoarceti la meniu? (1) Da"<<endl; int a; cin>>a;
                    }
                }
            }
        }

    }
    else {
        int c,c1;
        cout<<"De pe care carduri din cele existente in aplicatie doriti sa trimiteti bani? Introduceti numarul cardului:"<<endl; cin>>c;
        if(VerificareCard(c)){
            cout<<"Introduceti numarul cardului pe care banii sa fie adaugati:"<<endl; cin>>c1;
            if(VerificareCard(c1)){
                double s;
                cout<<"Introduceti suma pe care doriti sa o adaugati:"<<endl; cin>>s;

                for( auto &card : carduri){

                    if(c == card->getNrCard() && card->getCont()->getSuma() < s){
                        cout<<"Sold insuficient in cardul cu numarul "<<card->getNrCard()<<"."<<endl;
                        cout<<"Puteti adauga maxim "<<card->getCont()->getSuma()<<"lei."<<endl;
                    }
                    else if(c == card->getNrCard() ){
                        double sumaNoua= card->getCont()->getSuma() - s;
                        card->getCont()->setSuma(sumaNoua);
                        for(auto &card : carduri){
                            if(card->getNrCard() == c1){
                                double sumaN= card->getCont()->getSuma() + s;
                                card->getCont()->setSuma(sumaN);
                            }
                        }
                        cout<<" Suma a fost adaugata cu succes. Situatia financiara actuala a coturilor dvs. este:"<<endl;
                        VizualizareCarduri();
                        cout<<"Doriti sa va intoarceti la meniu? (1) Da"<<endl; int a; cin>>a;
                    }
                }
            }
        }
//
    }




}


};


void Meniu(){
    cout << "*******************  MENIU *******************" << endl;
    cout << endl;
    cout << "Bine ati venit in aplicatia dumneavoastra bancara!" << endl;
    cout << "(1) Intrati in cont           (2) Creeaza un cont" << endl;
    cout<<"              (3) Deconectare aplicatie"<<endl;
}


int main(){
    Aplicatie aplicatie;
    int opt;
    Meniu();
    while (cin >> opt) {
        switch(opt) {
            case 1: {
                string n, dn, p, pr;
                cout << "Introduceti numele de familie:" ; cin >> n;
                cout<< "Introduceti prenumele:"; cin>>pr;
                cout << "Introduceti data nasterii:" ; cin >> dn;
                cout << "Introduceti parola:" ;  cin >> p;
                aplicatie.inrareCont(n,pr, dn, p);
                Meniu();
                break;
            }
            case 2: {
                string n,pr, d, p;
                cout<<" !! Introduceti datele cu atentie, aplicatia poate sustine un singur utilizator !!"<<endl;
                cout << "Introduceti numele de familie:";cin >> n;
                cout<<"Introduceti prenumele:"; cin>>pr;
                cout << "Introduceti data nasterii:"; cin >> d;
                cout << "Introduceti parola:" ; cin >> p;


                try {
                    aplicatie.creeazaUtilizator(n,pr, d, p);

                } catch (const runtime_error& e) {
                    cout << e.what() << endl;
                }
                Meniu();
                break;
            }
            case 3:{
                cout<<"La revedere!"<<endl;
                return 0;
            }
            default: {
                cout << "Optiune invalida!" << endl;
                cout<<"Apasati (1) Intrare cont   (2) Creeare cont"<<endl;
                break;
            }
        }
    }

    return 0;
}
