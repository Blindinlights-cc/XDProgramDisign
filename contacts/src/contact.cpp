//
// Created by blindinlight on 4/3/22.
//
#define CONTACT_CONTACT_CPP
#include "contact.h"
#include<algorithm>
#include "global.h"

/************************************************************/
// Output Format

static std::vector<Contact> ContactList;
//Create a contacts list
void GetWidth()
{
    //String width
        name_wid=15;
        adress_wid=30;
        tele_wid =15;
    for(auto &p:ContactList){
        name_wid=name_wid>p.Name.size()?name_wid:p.Name.size();
        adress_wid=adress_wid>p.Address.size()?adress_wid:p.Address.size();
        tele_wid=tele_wid>p.Telephone.size()?adress_wid:p.Telephone.size();
    }
        sex_wid=7;
        age_wid=7;
        total_wid=name_wid+adress_wid+tele_wid
              +sex_wid+age_wid+5;
}
Status AddPerson()
{
    //Create a contact and add it to list
    Contact Person;
    int sex;
    while(true) {
        std::cout << "Please enter name: " << std::endl;
        getline(std::cin, Person.Name);
        if(Search(Person.Name)== ContactList.end())
            break;
        else
            std::cout<<"Contact exists!"<<std::endl;
    }
    std::cout<<"Please Enter telephone number: "<<std::endl;
    getline(std::cin,Person.Telephone);
    std::cout<<"Please enter address: "<<std::endl;
    getline(std::cin,Person.Address );
    std::cout<<"Please enter age: "<<std::endl;
    getline(std::cin,Person.Age);
    std::cout<<"Please chose sex 1) Male 2) Female (1 or 2):"<<std::endl;
    //Chose 1 or 2
    while(true){
        std::cin>>sex;
        if(sex==1){
            Person.Sex=Male;
            break;
        }
        else if (sex==2){
            Person.Sex=Female;
            break;
        }
        else
            std::cout<<"Please enter 1 or 2: ";
    }
    //Add to list
    ContactList.push_back(Person);
    //sort
    std::sort(ContactList.begin(),ContactList.end(),[](Contact &a,Contact &b){ return a.Name<b.Name;});
    GetWidth();
    PAUSE;
    return OK;

}
std::vector<Contact>::iterator Search(std::string &s)
{
    //search the name and return its iterator
    for(auto it=ContactList.begin();it!=ContactList.end();it++)
    {
        if((*it).Name==s)
            return it;
    }
    //if there is not the person return end()
    return ContactList.end();
}
Status SearchPerson()
{
    // search person and display
    std::string s;
    std::cout<<"Please enter the name you want to search: ";
    std::cin>>s;
    auto it =Search(s);
    if(it==ContactList.end()){
        // if not ,error
        std::cout<<"No counterpart,please check your name\n";
        PAUSE;
        return ERR;
    }
    else{
        //display the contact
        ShowHead();
        PrintContact(*it);
    }
    PAUSE;
    return OK;

}
Status DeletePerson()
{
    std::string s;
    std::cout<<"Please enter the name you want to Delete: ";
    std::cin>>s;
    auto it =Search(s);//search
    if(it==ContactList.end()){
        // if not,error
        std::cout<<"No counterpart,please check your name\n";
        PAUSE;
        return ERR;
    }
    else {
        //display the contact
        ShowHead();
        PrintContact(*it);
    }
    int Flag;
    // confirm
    std::cout<<"Do you want to delete? 1) yes  2) no: (1 or 2)";
    while(true){
        std::cin>>Flag;
        if(Flag==1){
            ContactList.erase(it);//delete
            break;
        }
        else if(Flag ==2){
            break;
        }
        else{
            std::cout<<"Please enter 1 or 2";
        }
    }
    PAUSE;
    return OK;

}
Status ShowHead()
{
    //header of the table
    table_t head{"Name","Telephone","Sex","Age","Address"};
    PrintBreak();
    std::cout.fill('-');
    PrintLine(head);
    return OK;
}
Status PrintLine(table_t const &tbl){
    std::cout.width(name_wid);
    std::cout<<("|"+tbl[0])<<'|';
    std::cout.width(tele_wid);
    std::cout<<(""+tbl[1])<<'|';
    std::cout.width(sex_wid);
    std::cout<<(""+tbl[2])<<'|';
    std::cout.width(age_wid);
    std::cout<<(""+tbl[3])<<'|';
    std::cout.width(adress_wid);
    std::cout<<(""+tbl[4])<<'|';
    std::cout<<"\n";
    PrintBreak();
    return OK;
}
void PrintBreak(){
    //horizontal line between contacts
    std::cout.width(total_wid);
    std::cout.fill('-');
    std::cout<<'-'<<'\n';
    std::cout.fill(' ');
}
Status PrintContact (Contact &c)
{
    //print the contact in a line
    std::string s;
    if(c.Sex==Male) s="Male";
    else s="Female";
    table_t line ={c.Name,c.Telephone,s,c.Age,c.Address};
    PrintLine(line);
    return OK;
}
Status DisplayContacts()
{
    //set out put format
    std::cout.setf(std::ios::left, std::ios::adjustfield);
    //display header o table
    ShowHead();
    // display each contact
    for(auto &p:ContactList)
        PrintContact(p);
    PAUSE;
    return OK;
}
Status Save()
{
    std::string filename;
    std::cout<<"Please enter file name: ";
    std::cin>>filename;
    std::ofstream fs(filename);
    if(!fs.is_open() ){
        //ERROR
        std::cerr<<"Failed to open. ";
        PAUSE;
        return ERR;
    }
    else
    {//write data
        fs.flush();
        for(auto const &p:ContactList)
            fs<<p<<"\n";
        std::cout<<"Writing........\n";
        std::cout<<"Success!";
    }
    PAUSE;
    return OK;
}
Status Load(){
    std::string filename;
    Contact p;
    std::cout<<"Please enter file name: ";
    std::cin>>filename;
    std::ifstream fs(filename);
    //fs.open(filename,std::ios_base::out);
    if(!fs.is_open() ){
        //ERROR
        std::cerr<<"Failed to open. ";
        PAUSE;
        return ERR;
    }
    else
    {
        // read data  add them to list
        while(fs>>p)
            ContactList.push_back(p);
        std::cout<<"Loading........\n";

        std::cout<<"Success!";
    }
    PAUSE;
    return OK;
}
std::ofstream &operator<<(std::ofstream &out,const Contact &a)
{
    out<<a.Name<<'\n';
    out<<a.Telephone<<'\n';
    out<<a.Address<<'\n';
    out<<a.Age<<'\n';
    out<<a.Sex<<'\n';

return out;
}
std::ifstream &operator>>(std::ifstream &in,Contact &a){
    int s;
    in>>a.Name;
    in>>a.Telephone;
    in>>a.Address;
    in>>a.Age;
    in>>s;
    if(s==1)a.Sex=Male;
    else a.Sex=Female;
    return in;
}

