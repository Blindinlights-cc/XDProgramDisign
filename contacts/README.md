## 项目一：通讯录

> **该系统主要功能包括：**
>
> 1. 录入通信录信息；
> 2.  实现删除功能，
> 3. 显示功能，显示通讯录中所有的信息；3
> 4. 实现查找功能，输入姓名显示对应信息；
> 5. 保存功能，将输入的通讯录信息保存到指定的磁盘文件中；
> 6. 加载功能，加载磁盘文件中保存的内容；
> 7. 退出通讯录管理系统。
>

首先要有主界面

用简单的标准输出实现

````c++
void ShowMenu(){
    //MainMenu
    system("clear");//刷新界面，Windows下为“cls”
    std::cout<<"1) Create a contact\n";
    std::cout<<"2) Delete a contact\n";
    std::cout<<"3) Show all contacts\n";
    std::cout<<"4) Search the contact\n";
    std::cout<<"5) Save contacts\n";
    std::cout<<"6) Load contacts\n";
    std::cout<<"7) Quit contacts\n";
}
````

主函数中包括选择模块

输入数字，启动相应的功能。switch实现。

每一个功能对应一个函数用以实现该功能。

````c++
Status AddPerson(); // 添加联系人
Status DeletePerson();//删除联系人
Status SearchPerson();// 查找联系人
Status DisplayContacts();//展示联系人
Status Save();			//保存
Status Load();			//加载
````

主函数的内容就此完成，如下

````c++
int main ()
{
    std::ios::sync_with_stdio(false);
    //关闭流同步
    ShowMenu();//展示主菜单
    int a;
    while(true){
        std::cin>>a;
        std::cin.get();
        switch (a){
            case 1:
                AddPerson();
                ShowMenu(); //每次执行完后进入主菜单
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
````

下面考虑具体功能的实现

**基本数据类型：联系人**

联系人看做一种数据类型，大致具有以下属性

- 姓名
- 年龄
- 性别
- 住址
- 电话号码

在c++中用类实现这一数据类型

````c++
class Contact{
public:
    std::string Name;
    std::string Address;
    std::string Telephone;
    sex Sex;
    std::string Age;
}
````

其中性别看做一种枚举

````c++
enum sex{
    Male=1,
    Female=2
};
````

联系人这一数据类型的实现结束

由于是简单的通讯录，可以用线性表这一数据结构储存数据

c++中可以使用std::vector

```c++
static std::vector<Contact> ContactList;
```

Contact List即为通讯录。

**查找**

因为搜索、删除、添加联系人都需要查找这一操作，因而先实现根据name查找联系人这一功能；

查找的参数应为字符串，返回值应该是一个指向对应联系人的迭代器，如果没有找到则返回end()。

具体实现：

````c++
std::vector<Contact>::iterator Search(std::string &s)
{

    for(auto it=ContactList.begin();it!=ContactList.end();it++)
    {
        if((*it).Name==s)
            return it;
    }
    return ContactList.end();
}
````



**添加联系人**

使用标准输入读写，并加入联系人,添加一些引导语

最后将联系人加入List

````c++
Status AddPerson()
{
    Contact Person;
    int sex;
    while(true) {
        std::cout << "Please enter name: " << std::endl;
        getline(std::cin, Person.Name);
        if(Search(Person.Name)== ContactList.end())
            break;
        else
            std::cout<<"Contact exists!"<<std::endl;
    }//不允许重名联系人
    std::cout<<"Please Enter telephone number: "<<std::endl;
    getline(std::cin,Person.Telephone);
    std::cout<<"Please enter address: "<<std::endl;
    getline(std::cin,Person.Address );
    std::cout<<"Please enter age: "<<std::endl;
    getline(std::cin,Person.Age);
    std::cout<<"Please chose sex 1) Male 2) Female (1 or 2):"<<std::endl;
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
    ContactList.push_back(Person);
    std::sort(ContactList.begin(),ContactList.end(),[](Contact &a,Contact &b){ return a.Name<b.Name;});    //按照字典序排序
    PAUSE;
    return OK;
}
````

**展示联系人**

在搜索，删除，展示联系人的功能中都有显示联系人信息这一功能，先实现这一功能。

考虑用表格的形式展示

那么表格分为两部分

- 表头
- 联系人信息



````c++
Status ShowHead();
Status PrintContact (Contact &s);
````

每行之间用水平线间隔

具体实现可分为两部分:打印信息，打印水平线；

用数组储存每行的信息并打印

````c++
using table_t=std::array<std::string,5>;
Status PrintLine(table_t const &tbl);
void PrintBreak();
````

实现：

````c++
void PrintBreak(){
    //水平线
    std::cout.width(total_wid);
    std::cout.fill('-');
    std::cout<<'-'<<'\n';
    std::cout.fill(' ');
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
Status ShowHead()
{
    //打印表头
    table_t head{"Name","Telephone","Sex","Age","Address"};
    PrintBreak();
    std::cout.fill('-');
    PrintLine(head);
    return OK;
}
Status PrintContact (Contact &c)
{
    //打印联系人
    std::string s;
    if(c.Sex==Male) s="Male";
    else s="Female";
    table_t line ={c.Name,c.Telephone,s,c.Age,c.Address};
    PrintLine(line);
    return OK;
}
````

其中的name_wid是每类数据所占用的宽度

**删除联系人**

查找相应的联系人，遍历整个列表，寻找name相同的联系人，返回其迭代器，展示联系人信息，确认用erase()删除。

导图：

````mermaid
graph LR
a["开始"]-->A
A["查找联系人"]-->|遍历列表|B{"找到了联系人"}
B-->|是|C["展示联系人"]
C-->D{"删除"}
D-->|是|E["用erase()删除"]
D-->|否|f
B-->|否|G[返回ERROR]
G-->F[结束]
E-->f[返回OK]
f-->F
````



加入引导语，未找到联系人返回ERROR

````c++
Status DeletePerson()
{
    std::string s;
    std::cout<<"Please enter the name you want to Delete: ";
    std::cin>>s;
    auto it =Search(s);
    if(it==ContactList.end()){
        std::cout<<"No counterpart,please check your name\n";
        PAUSE;
        return ERR;
    }
    else {
        ShowHead();
        PrintContact(*it);
    }
    int Flag;
    std::cout<<"Do you want to delete? 1) yes  2) no: (1 or 2)";//确认
    while(true){
        std::cin>>Flag;
        if(Flag==1){
            ContactList.erase(it);
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
````

**搜索联系人**

逻辑大体上和删除差不多

````mermaid
graph LR
a["开始"]-->A
A["查找联系人"]-->|遍历列表|B{"找到了联系人"}
B-->|是|C["展示联系人"]
C-->f
B-->|否|G[返回ERROR]
G-->F[结束]
f["返回OK"]-->F
````

````c++
Status SearchPerson()
{
    std::string s;
    std::cout<<"Please enter the name you want to search: ";
    std::cin>>s;
    auto it =Search(s);
    if(it==ContactList.end()){
        std::cout<<"No counterpart,please check your name\n";
        PAUSE;
        return ERR;
    }
    else{
        ShowHead();
        PrintContact(*it);
    }
    PAUSE;
    return OK;

}
````

**展示联系人**

综合一下之前展示的部分，打印表头和每个联系人即可

````c++
Status DisplayContacts()
{
    std::cout.setf(std::ios::left, std::ios::adjustfield);//左对齐，填充
    ShowHead(); //打印表头
    for(auto &p:ContactList)
        PrintContact(p);//逐个打印联系人
    PAUSE;
    return OK;
}
````

**保存和展示联系人**

首先定制联系人类操作在fstream下的>>和<<

````c++
    friend std::ofstream &operator<<(std::ofstream &out,const Contact &a){
        out<<a.Name<<'\n';
        out<<a.Telephone<<'\n';
        out<<a.Address<<'\n';
        out<<a.Age<<'\n';
        out<<a.Sex<<'\n';

        return out;
    }
    friend std::ifstream &operator>>(std::ifstream &in,Contact &a){
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

````

利用定制操作实现保存和加载,加入一些引导语

````c++
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
````

将内容整合至多个文件中编译运行成功实现！
