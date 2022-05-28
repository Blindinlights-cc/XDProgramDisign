#ifndef DATABASE_HPP
#define DATABASE_HPP
#include "statement.hpp"
#include <array>
#include <cstddef>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#define  _int32 0
#define  _float32 1
#define  _string 2
namespace Blinding {
    class Table{
    public:
        std::string table_name;                                        //表名
        std::vector<std::string> header;
        std::map<std::string,int32_t> col;                        //表头
        std::map<std::string,std::vector<std::string>> vars;         
        uint32_t records_num=0;
        uint32_t col_num=0;
        std::string_view path="/home/blindinglights/minidb/tables/";
        std::string db;

        Table()=default;
        //Table(std::string &name,bool flag):table_name(name){ read();}
                                                                    //构造函数，根据table名读取数据
        ~Table()= default;;
        ReadTable read();                                           //读取文件中的数据
        //ReadTable read(std::string &name);
        WriteTable write();                                         //储存数据到磁盘
        void remove_file();

    };

    class Database{
    public:
        std::string db_name;                                           //数据库名
        std::vector<Table> tables;                               //表
        uint16_t tables_nums=0;                                       //表数
        std::string_view path="/home/blindinglights/minidb/databases/";
        
        Database()=default;
        explicit Database(std::string &name):db_name(name){}
        Database(std::string &name,bool flag):db_name(name){ this->read();};
        ~Database()= default;;

        ReadDatabase read();
        ReadDatabase load();
        //ReadDatabase read(std:: string &name);                      //读取数据
        WriteDatabase write();                                      //储存数据到磁盘
        void remove_file();
    };

    
}



#endif