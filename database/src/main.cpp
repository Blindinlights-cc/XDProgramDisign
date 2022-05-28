#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include "database.hpp"
#include "statement.hpp"
#define Match(a) do{switch ((a)){case EXECUTE_SUCCESS:  return PREPARE_SUCCESS;    case EXECUTE_FAILED:  return PREPARE_EXECUTE_ERROR;  } }while(0)



using Blinding::Database;
using Blinding::Table;
std::vector<std::string>db_names;


ExecuteResult 
create_db(std::string &name);

auto
search_db(std::string &name){
    auto it=std::find(db_names.begin(),db_names.end(),name);
    return it;
    
}
auto
search_tb(Database &in_use,std::string &name){
    return std::find_if(in_use.tables.begin(),in_use.tables.end(),
                            [name](Table &a){return a.table_name==name;});
    
}



ExecuteResult
read_file()
{
    std::fstream file("/home/blindinglights/minidb/base.bldb",std::ios::in);
    if(!file.is_open()){
        std::cout<<"Failed to read data file.\n";
        return EXECUTE_FAILED;
    }
    std::string name_get_from_file;
    while(getline(file,name_get_from_file)){
        db_names.push_back(name_get_from_file);
    }
    system("clear");
    return EXECUTE_SUCCESS;

}
ExecuteResult
write_file()
{
    std::fstream file("/home/blindinglights/minidb/base.bldb",std::ios::out);
    if(!file.is_open()){
        std::cout<<"Failed to write data file.\n";
        return EXECUTE_FAILED;
    }
    file.flush();
    for(auto &p:db_names){
        file<<p<<"\n";
    }
    return EXECUTE_SUCCESS;
}



ExecuteResult 
create_db(std::string &name){                                    //execute 'create database [name]'.
    auto x=new Database;
    auto flag=search_db(name);
    if(flag!=db_names.end()){
        std::cout<<"Database "<<name<<" exists. Filed to create.\n";
        return EXECUTE_FAILED;
    }
    db_names.push_back(name);
    x->db_name=name;
    x->write();
    delete x;
    write_file();
    std::cout<<"Database "<<name<<" is created.\n";
    return EXECUTE_SUCCESS;
}


ExecuteResult
create_tb(Database &in_use,std::string &name,std::vector<std::pair<std::string,int32_t>> &cols_header){                   //execute 'create table [name] ([col_type][col_name])'.
    auto flag=search_tb(in_use,name);
    if(flag!=in_use.tables.end()){
        std::cout<<"Table "<<name<<" exists from "<<in_use.db_name<<" .Filed to create.\n";
        return EXECUTE_FAILED;
    }

    Table foo;
    foo.table_name=name;
    foo.col_num=cols_header.size();
    for(auto &p:cols_header){
        foo.header.push_back(p.first);
        foo.col[p.first]=p.second;
    }
    foo.db=in_use.db_name;
    foo.write();
    in_use.tables.push_back(foo);
    in_use.tables_nums++;
    in_use.write();
    std::cout<<"Table "<<name<<" is created;\n";
    return EXECUTE_SUCCESS;
}
ExecuteResult
use_db(Database &in_use,std::string &name){                     //execute 'use [name]'. Set a database as in-used db.
    auto flag =search_db(name);
    if(flag!=db_names.end()){
        in_use.db_name=name;
        in_use.read();
        std::cout<<"Now using database "<<name<<".\n";
        return EXECUTE_SUCCESS;
    }else{
        std::cout<<"No such database.\n";
        return EXECUTE_FAILED;
    }
    
  
}

PrepareResult
praser_cols(Database &in_use, std::string &cols){
    std::stringstream ss(cols);
    std::string table_name;
    std::string buffer1;
    std::string buffer2;
    std::string buffer;
    std::vector<std::pair<std::string, int32_t>> cols_header;
    getline(ss,table_name,'(');
    std::stringstream d(table_name);
    d>>table_name;
    while(getline(ss,buffer,',')){
        for(auto &p:buffer)
            if(p=='('||p==')') p=' ';
        std::stringstream decl(buffer);
        decl>>buffer1;
        decl>>buffer2;
        if(buffer1=="int")
            cols_header.emplace_back(buffer2,0);
        else if(buffer1=="float")
            cols_header.emplace_back(buffer2,1);
        else if(buffer1=="string")
            cols_header.emplace_back(buffer2,3);
        else{
            std::cout<<"Type error.Please check your input.(supported type: int,float,string)\n";
            return PREPARE_SYNTAX_ERROR;
        }
    }
    Match(create_tb(in_use,table_name,cols_header));
    
} 



ExecuteResult
drop_database(std::string &name){

    auto flag=search_db(name);
    if(flag!=db_names.end()){
        auto temp=new Database;
        temp->db_name=name;
        temp->remove_file();
        delete temp;
        db_names.erase(flag);
    }else{
        std::cout<<"No such database.\n";
        return EXECUTE_FAILED;
    }
    write_file();
    std::cout<<"Drop successfully.\n";
    return EXECUTE_SUCCESS;
}



ExecuteResult
drop_table(Database &in_used, std::string &name){
    auto flag= search_tb(in_used,name);
    if(flag==in_used.tables.end()){
        std::cout<<"No table names "<<name<<".\n";
        return EXECUTE_FAILED;
    }
    flag->remove_file();
    in_used.tables.erase(flag);
    in_used.tables_nums--;
    std::cout<<"Table "<<name<<" is dropped.\n";
    return EXECUTE_SUCCESS;
}
void print_break(unsigned int total_width){
    std::cout.width(total_width);
    std::cout.fill('-');
    std::cout<<'-'<<'\n';
    std::cout.fill(' ');
}

ExecuteResult
info(Table &tb){
    unsigned int total_width=tb.header.size()*16+1;
    print_break(total_width);
    std::cout<<'|';
    for(auto &p:tb.header){
        std::cout.width(15);
        std::cout<<(""+p)<<'|';
    }
    std::cout<<"\n";
    print_break(total_width);
    for(unsigned  int i=0;i<tb.records_num;i++){
        std::cout<<'|';
        for(auto &p:tb.header){
            std::cout.width(15);
            std::cout<<(""+(tb.vars[p])[i])<<'|';
        }
        std::cout<<"\n";
        print_break(total_width);
    }
    return EXECUTE_SUCCESS;
}

ExecuteResult
info_from_table(Database &in_used, std::string &name){
    auto flag=search_tb(in_used,name);
    if(flag==in_used.tables.end()){
        std::cout<<"No such table.\n";
        return EXECUTE_FAILED;
    }
    return info((*flag));
}


auto
search_record(std::vector<Blinding::Table>::iterator &it,std::string &col_name,char operation,std::string &value){
    std::vector<size_t> tar;
    if(operation=='='){
        for(size_t i=0;i<it->records_num;i++)
        {
            if(it->vars[col_name][i]==value)
                tar.push_back(i);
        }
    } else if(operation=='<'){
        if(it->col[col_name]==_int32){
            for(size_t i=0;i<it->records_num;i++)
            {
                if(std::stoi(it->vars[col_name][i])<std::stoi(value))
                    tar.push_back(i);
            }
        } else if(it->col[col_name]==_float32){
            for(size_t i=0;i<it->records_num;i++)
            {
                if(std::stof(it->vars[col_name][i])<std::stof(value))
                    tar.push_back(i);
            }
        } else{
            for(size_t i=0;i<it->records_num;i++)
            {
                if(it->vars[col_name][i]<value)
                    tar.push_back(i);
            }
        }
    }else if(operation=='>'){
        if(it->col[col_name]==_int32){
            for(size_t i=0;i<it->records_num;i++)
            {
                if(std::stoi(it->vars[col_name][i])>std::stoi(value))
                    tar.push_back(i);
            }
        } else if(it->col[col_name]==_float32){
            for(size_t i=0;i<it->records_num;i++)
            {
                if(std::stof(it->vars[col_name][i])>std::stof(value))
                    tar.push_back(i);
            }
        } else{
            for(size_t i=0;i<it->records_num;i++)
            {
                if(it->vars[col_name][i]>value)
                    tar.push_back(i);
            }
        }
    }
    return tar;
}
auto
search_record(Table *it,std::string &col_name,char operation,std::string &value){
    std::vector<size_t> tar;
    if(operation=='='){
        for(size_t i=0;i<it->records_num;i++)
        {
            if(it->vars[col_name][i]==value)
                tar.push_back(i);
        }
    } else if(operation=='<'){
        if(it->col[col_name]==_int32){
            for(size_t i=0;i<it->records_num;i++)
            {
                if(std::stoi(it->vars[col_name][i])<std::stoi(value))
                    tar.push_back(i);
            }
        } else if(it->col[col_name]==_float32){
            for(size_t i=0;i<it->records_num;i++)
            {
                if(std::stof(it->vars[col_name][i])<std::stof(value))
                    tar.push_back(i);
            }
        } else{
            for(size_t i=0;i<it->records_num;i++)
            {
                if(it->vars[col_name][i]<value)
                    tar.push_back(i);
            }
        }
    }else if(operation=='>'){
        if(it->col[col_name]==_int32){
            for(size_t i=0;i<it->records_num;i++)
            {
                if(std::stoi(it->vars[col_name][i])>std::stoi(value))
                    tar.push_back(i);
            }
        } else if(it->col[col_name]==_float32){
            for(size_t i=0;i<it->records_num;i++)
            {
                if(std::stof(it->vars[col_name][i])>std::stof(value))
                    tar.push_back(i);
            }
        } else{
            for(size_t i=0;i<it->records_num;i++)
            {
                if(it->vars[col_name][i]>value)
                    tar.push_back(i);
            }
        }
    }
    return tar;
}


ExecuteResult
delete_from_table(std::vector<Blinding::Table>::iterator &it,std::string &col_name,char operation,std::string &value){
    auto flag= std::find(it->header.begin(), it->header.end(),col_name);
    if(flag==it->header.end()){
        std::cout<<"No such field.\n";
        return EXECUTE_FAILED;
    }
    auto x= search_record(it,col_name,operation,value);
    for(auto &p:it->vars){
        for(auto i:x){
            p.second[i]="#####";
        }
    }
    for(auto &p:it->vars)
        p.second.erase(std::remove(p.second.begin(),p.second.end(),"#####"),p.second.end());
    it->records_num-=x.size();
    it->write();
    return EXECUTE_SUCCESS;
}
ExecuteResult
delete_from_table( Table*it,std::string &col_name,char operation,std::string &value){
    auto flag= std::find(it->header.begin(), it->header.end(),col_name);
    if(flag==it->header.end()){
        std::cout<<"No such field.\n";
        return EXECUTE_FAILED;
    }
    auto x= search_record(it,col_name,operation,value);
    for(auto &p:it->vars){
        for(auto i:x){
            p.second[i]="#####";
        }
    }
    for(auto &p:it->vars)
        p.second.erase(std::remove(p.second.begin(),p.second.end(),"#####"),p.second.end());
    it->records_num-=x.size();
    it->write();
    return EXECUTE_SUCCESS;
}
ExecuteResult
delete_from_table(std::vector<Blinding::Table>::iterator &it){
    for(auto &p:it->vars){
        p.second.clear();
    }
    it->records_num=0;
    it->write();
    std::cout<<"Delete successfully.\n";
    return EXECUTE_SUCCESS;
}

PrepareResult
parser_delete(Database &in_used,std::string &name,std::string &condition){
    auto flag= search_tb(in_used,name);
    if(flag==in_used.tables.end()){
        std::cout<<"No such table.Please check your input.\n";
        return PREPARE_SYNTAX_ERROR;
    }
    //condition.erase(std::remove(condition.begin(),condition.end(),' '),condition.end());
    if(condition.empty()){
        Match(delete_from_table(flag));
    }
    std::stringstream ss(condition);
    std::string buffer;
    std::string col_name;
    std::string value;
    ss>>buffer;
    if(buffer!="where")return PREPARE_UNRECOGNIZED_STATEMENT;
    ss>>buffer;
    auto n=buffer.find_first_of('<');
    if(n==std::string::npos){
            n=buffer.find_first_of('=');
            if(n==std::string::npos) {
                n = buffer.find_first_of('<');
                if (n == std::string::npos)
                    return PREPARE_SYNTAX_ERROR;
            }
    }
    col_name=buffer.substr(0,n);
    value=buffer.substr(n+1);
    value.erase(std::remove(value.begin(),value.end(),'\''),value.end());
    Match(delete_from_table(flag,col_name,buffer[n],value));
}

ExecuteResult
insert_from_table( std::vector<Blinding::Table>::iterator &it,std::vector<std::pair<std::string,std::string>> &data){
    for(auto &p:data){
        (it->vars[p.first]).push_back(p.second);
    }
    it->records_num++;
    it->write();
    return EXECUTE_SUCCESS;

}
PrepareResult
parser_insert(Database &in_use,std::string &tb,std::string &arg){
    auto flag= search_tb(in_use,tb);
    if(flag==in_use.tables.end()){
        std::cout<<"No such table.Please check your input.\n";
        return PREPARE_SYNTAX_ERROR;
    }
    std::vector<std::pair<std::string,std::string>> data;
    std::string buffer;
    std::string buffer1;
    std::string buffer2;
    std::stringstream ss(arg);
    unsigned int i=0;
    while(getline(ss,buffer,',')){
        std::stringstream del(buffer);
        getline(del,buffer1,'=');
        getline(del,buffer2);
        buffer1.erase(std::remove(buffer1.begin(),buffer1.end(),' '),buffer1.end());
        buffer2.erase(std::remove(buffer2.begin(),buffer2.end(),' '),buffer2.end());
        if(i>=flag->header.size()){
            return PREPARE_UNRECOGNIZED_STATEMENT;
        }
        if(flag->header[i]!=buffer1){
            std::cout<<"Cols doesn't match.\n";
            return PREPARE_SYNTAX_ERROR;
        }
        buffer2.erase(std::remove(buffer2.begin(),buffer2.end(),'\''),buffer2.end());
        data.emplace_back(std::make_pair(buffer1,buffer2));
        i++;
    }
    Match(insert_from_table(flag,data));
}

ExecuteResult
select_from_table(std::vector<Blinding::Table>::iterator &it, std::vector<std::string>  &cols,
                  std::vector<std::tuple<std::string,char,std::string>> &conditions,std::string &order_by,bool order){
    auto x=new Table;
    (*x)=*it;
    if(!conditions.empty()){
        for(auto &p:conditions){
            if(std::get<1>(p)=='>'){
                delete_from_table(x, std::get<0>(p),'=',std::get<2>(p));
                delete_from_table(x, std::get<0>(p),'<',std::get<2>(p));
            } else if(std::get<1>(p)=='<'){
                delete_from_table(x, std::get<0>(p),'=',std::get<2>(p));
                delete_from_table(x, std::get<0>(p),'>',std::get<2>(p));
            }else if(std::get<1>(p)=='='){
                delete_from_table(x, std::get<0>(p),'>',std::get<2>(p));
                delete_from_table(x, std::get<0>(p),'<',std::get<2>(p));
            }
        }
    }
    int num=0;
    if(cols.empty())
        cols=x->header;
    for(int i=0;i<cols.size();i++){
        if(cols[i]==order_by)
            num=i;
    }

    auto a=[num,order](std::vector<std::string> &f,std::vector<std::string> &l)
        {   if(order)
            return (f[num])<(l[num]);
            else
            return (f[num])>(l[num]);
        };

    auto b=[num,order](std::vector<std::string> &f,std::vector<std::string> &l)
        {   if(order)
            return std::stoi(f[num])<std::stoi(l[num]);
            else
            return std::stoi(f[num])>std::stoi(l[num]);
        }  ;

    auto c=[num,order](std::vector<std::string> &f,std::vector<std::string> &l)
            {   if(order)
                return std::stof(f[num])<std::stof(l[num]);
                else
                return std::stof(f[num])>std::stof(l[num]);
            };



    std::vector<std::vector<std::string>> tar;
    for(auto i=0;i<x->records_num;i++){
        std::vector<std::string> t;
        for(auto &p:cols){
            t.push_back(x->vars[p][i]);
        }
        tar.push_back(t);
        t.clear();
    }
    if(!order_by.empty()){
        if(x->col[order_by]==_string){
            std::sort(tar.begin(),tar.end(),a);
        }
        else if(x->col[order_by]==_int32){
            std::sort(tar.begin(),tar.end(),b);
        }
        else if(x->col[order_by]==_float32){
            std::sort(tar.begin(),tar.end(),c);
        }

    }

    delete x;
    auto y= new Table;
    y->header=cols;
    for(auto &i:tar){
        for(auto &p:cols){
            y->vars[p]=i;
        }
    }
    y->records_num=tar.size();
    y->col_num=tar[0].size();
    info(*y);

    return EXECUTE_SUCCESS;
}


PrepareResult
parser_select(Database &in_used,std::string &statement){
    std::vector<std::string> words;
    std::stringstream ss(statement);
    std::string buffer;
    while (ss>>buffer)
        words.push_back(buffer);
    auto it= std::find(words.begin(), words.end(),"from");
    if(it==words.end()||it==words.begin()+1||it==words.end()-1){
        return PREPARE_UNRECOGNIZED_STATEMENT;
    }
    std::string cols_str(*(it-1));
    std::string name(*(it+1));
    std::vector<std::string> cols;
    std::vector<std::tuple<std::string,char,std::string>> conditions;
    if(cols_str=="*"){
        cols.clear();
    } else{
        std::stringstream c(cols_str);
        while (getline(c,buffer,','))
            cols.push_back(buffer);
    }
    bool order= false;
    std::string order_by;
    auto flag= search_tb(in_used,name);
    if(flag==in_used.tables.end())
        return PREPARE_SYNTAX_ERROR;
    std::vector<std::string> conditions_str;
    if((it+2)==words.end())
        conditions.clear();
    else{
        if(*(it+2)!="where")
            return PREPARE_UNRECOGNIZED_STATEMENT;
        it+=3;
        std::stringstream con(*it);
        while(getline(con,buffer,','))
            conditions_str.push_back(buffer);
        it++;
        order_by.clear();
        if(it==words.end())
            order_by.clear();
        else{
            if(*it!="order")
                return PREPARE_UNRECOGNIZED_STATEMENT;
            it++;
            if(it==words.end()||*it!="by")
                return PREPARE_UNRECOGNIZED_STATEMENT;
            it++;
            if(it==words.end())
                return PREPARE_SYNTAX_ERROR;
            order_by.append(*it);
            it++;
            if(it==words.end())
                return PREPARE_SYNTAX_ERROR;
            if(*it=="desc")
                order=true;
            else
                order=false;

        }
    }
    for(auto &p:cols){
        auto x= std::find(flag->header.begin(), flag->header.end(),p);
        if(x==flag->header.end()) {
            std::cout << "No such field.\n";
            return PREPARE_SYNTAX_ERROR;
        }
    }
    auto y= std::find(cols.begin(), cols.end(),order_by);
    if(y==cols.end()){
        std::cout<<"Can't order by "<<order_by<<".\n";
        return PREPARE_SYNTAX_ERROR;
    }
    for(auto &p:conditions_str){
        auto n=p.find_first_of('<');
        if(n==std::string::npos){
            n=p.find_first_of('=');
            if(n==std::string::npos) {
                n = p.find_first_of('<');
                if (n == std::string::npos)
                    return PREPARE_SYNTAX_ERROR;
            }
        }
        conditions.emplace_back(p.substr(0,n),p[n],p.substr(n+1));

    }
    Match(select_from_table(flag,cols,conditions,order_by,order));
}


PrepareResult
prepare(std::string &cmd,  Database &in_use)
{
    std::stringstream ss(cmd);
    std::string str;
    ss>>str;
    if(str=="create"){                                                  //Branch ’create‘ 
        ss>>str;
        if (str=="table"){                                              //create table [name]*
            if(in_use.db_name=="$$$$$$")return PREPARE_EMPTY_INUSE;
                getline(ss,str);
                return praser_cols(in_use,str);


        } else if(str =="database"){
            ss>>str;
            if (str.empty()){                                           //create database [name]
                std::cout<<"Syntax Error.Please input name.\n";
                return PREPARE_SYNTAX_ERROR;
            }
            Match(create_db(str));
        } else{
            
            return PREPARE_UNRECOGNIZED_STATEMENT;
        }

    } else if(str=="drop"){                                             //Branch 'drop'
        ss>>str;
        if(str=="database"){
            ss>>str;
            Match(drop_database(str));

        } else if(str=="table"){
            ss>>str;
            if(str.empty()){
                std::cout<<"Please input name.\n";
                return PREPARE_SYNTAX_ERROR;
            }
            if(in_use.db_name=="$$$$$$") {
                std::cout<<"Please select a database first!\n";
                return PREPARE_SYNTAX_ERROR;
            }
            
            Match(drop_table(in_use,str));
        }else{
            
            return PREPARE_UNRECOGNIZED_STATEMENT;
        }

    }else if(str=="delete"){                                            //Branch 'delete'
        if(in_use.db_name=="$$$$$$")return PREPARE_EMPTY_INUSE;
        ss>>str;
        if(str!="from")return  PREPARE_UNRECOGNIZED_STATEMENT;
        std::string name;
        ss>>name;
        if(!getline(ss,str))
            str.clear();
        return parser_delete(in_use,name,str);
    }else if(str=="use"){                                               //Branch 'use'
        ss>>str;
        if(str.empty()){
            std::cout<<"Please input database name.\n";
        }
        Match(use_db(in_use,str));
    }else if(str=="info"){                                              //Branch 'info'
        if(in_use.db_name=="$$$$$$")return PREPARE_EMPTY_INUSE;
        ss>>str;
        if(str=="table"){
            ss>>str;
            Match(info_from_table(in_use,str));
        } else{
            return  PREPARE_UNRECOGNIZED_STATEMENT;
        }

        return PREPARE_SUCCESS;

    } else if(str=="insert"){                                           //Branch 'insert'
        if(in_use.db_name=="$$$$$$")return PREPARE_EMPTY_INUSE;
            ss>>str;
            if(str=="into"){
                std::string name;
                ss>>name;
                ss>>str;
                if(str=="set"){
                    getline(ss,str);
                    return parser_insert(in_use,name,str);
                } else
                    return PREPARE_UNRECOGNIZED_STATEMENT;
            } else{
                return PREPARE_UNRECOGNIZED_STATEMENT;
            }

    } else if(str=="select"){                                           //Branch 'select'
        if(in_use.db_name=="$$$$$$")return PREPARE_EMPTY_INUSE;
        getline(ss,str);
        return parser_select(in_use,str);

    }
        return PREPARE_UNRECOGNIZED_STATEMENT;
}


int main (){

    read_file();
    std::cout<<"Welcome to my database!\n\n\n";
    std::cout<< R"(___.   .__        .___       __        ___.                  )"<<"\n"<<
                R"(\_ |__ |  |     __| _/____ _/  |______ \_ |__ _____    ______ ____  )"<<"\n"<<
                R"( | __ \|  |    / __ |\__  \\   __\__  \ | __ \\__  \  /  ___// __ \ )"<<"\n"<<
                R"( | \_\ \  |__ / /_/ | / __ \|  |  / __ \| \_\ \/ __ \_\___ \\  ___/ )"<<"\n"<<
                R"( |___  /____/ \____ |(____  /__| (____  /___  (____  /____  >\___  >)"<<"\n"<<
                R"(     \/            \/     \/          \/    \/     \/     \/     \/ )"<<"\n\n";

    std::string cmd;
    Database in_use;
    in_use.db_name="$$$$$$";
    while(true){
        std::cout<<"\nBLdb> ";
        getline(std::cin,cmd,';');
        if(cmd==".exit") {
            std::cout<<"Bye!";
            break;
        }
        else {
            switch (prepare(cmd,in_use)) {

                case PREPARE_SUCCESS:std::cout<<"Execute successfully.\n";
                    break;
                case PREPARE_SYNTAX_ERROR:std::cout<<"Syntax error.Please check your input.\n";
                    break;
                case PREPARE_EMPTY_INUSE:std::cout<<"Use a database first!\n";
                    break;
                case PREPARE_UNRECOGNIZED_STATEMENT:std::cout<<"Unrecognized statement.\n";
                    break;
                case PREPARE_EXECUTE_ERROR:std::cout<<"Execute failed";
                    break;
            }
        }
    }
    return 0;
}
