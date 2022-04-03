//
// Created by blindinlight on 4/3/22.
//

#ifndef CONTACT_CONTACT_H
#define CONTACT_CONTACT_H


#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<array>
#include "global.h"
#define Status bool
#define OK true
#define ERR false
#define PAUSE do {std::cin.get(); std::cout<<"\nPress any key to continue...."; std::cin.get(); }while(0)     //pause and stay part
enum sex{
    Male=1,
    Female=2
};

class Contact{
public:
    std::string Name;
    std::string Address;
    std::string Telephone;
    sex Sex;
    std::string Age;
    Contact()=default;
    friend std::ofstream &operator<<(std::ofstream &out,const Contact &a);
    friend std::ifstream &operator>>(std::ifstream &in,Contact &a);
};
using table_t=std::array<std::string,5>;

//Function declaration
Status AddPerson();
Status DeletePerson();
Status SearchPerson();
std::vector<Contact>::iterator Search(std::string &s);
Status DisplayContacts();
Status ShowHead();
Status PrintContact (Contact &s);
Status PrintLine(table_t const &tbl);
void PrintBreak();
void GetWidth();
Status Save();
Status Load();

/***********************************************************/


#endif //CONTACT_CONTACTS_H
