#include<iostream>
#include"contact.h"
#pragma Optimize(2)
void ShowMenu(){
    //MainMenu
    system("clear");
    std::cout<<"1) Create a contact\n";
    std::cout<<"2) Delete a contact\n";
    std::cout<<"3) Show all contacts\n";
    std::cout<<"4) Search the contact\n";
    std::cout<<"5) Save contacts\n";
    std::cout<<"6) Load contacts\n";
    std::cout<<"7) Quit contacts\n";
}
int main ()
{
    std::ios::sync_with_stdio(false);
    ShowMenu();
    int a;
    while(true){
        std::cin>>a;
        std::cin.get();
        switch (a){
            case 1:
                AddPerson();
                ShowMenu();
                break;
            case 2:
                DeletePerson();
                ShowMenu();
                break;
            case 3:
                DisplayContacts();
                ShowMenu();
                break;
            case 4:
                SearchPerson();
                ShowMenu();
                break;
            case 5:
                Save();
                ShowMenu();
                break;

            case 6:
                Load();
                ShowMenu();
                break;
            case 7:
                std::cout<<"See you !";
                return 0;
            default:
                break;
        }

    }
}