#include "database.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
namespace Blinding {
    ReadTable Table::read() {
        std::string filename(this->path);
        filename.append(this->table_name);
        filename.push_back('.');
        filename.append(this->db);
        filename.append(".csv");

        std::fstream out_file(filename,std::ios::in);
        if(!out_file.is_open()) return FAILED_TO_OPEN_FILE_TABLE;//文件打开失败，返回错误
        std::string line;                                       //每行字符
        std::string str;                                        //单个数据


        getline(out_file,line);                     //获取表头行
        std::stringstream  ss(line);                //用逗号分隔
        while(getline(ss,str,',')){                 //获取每个字段
            if(this->col_num>=10) return COl_NOT_MATCH; //最多十个col
            this->header.push_back(str);                //add cols
            this->col_num++;
        }



        getline(out_file,line);                 //get col type
        std::stringstream  sss(line);
        for(uint32_t i=0;i<this->col_num;i++){
            getline(sss,str,',');
            this->col[this->header[i]]= stoi(str);//use int numbers as cols type
        }
        while (getline(out_file,line)){             //get records
            std::stringstream buffer(line);
            for(uint32_t i=0;i<col_num;i++){
                getline(buffer,str,',');
                (this->vars[this->header[i]]).push_back(str) ;
            }
            this->records_num++;
        }
        return READ_SUCCESS;

    }
    WriteTable Table::write() {
        std::string filename(this->path);
        filename.append(this->table_name);
        filename.push_back('.');
        filename.append(this->db);
        filename.append(".csv");
        std::fstream out_file(filename,std::ios_base::out);
        out_file.flush();
        if(!out_file.is_open()) return FAILED_TO_OPEN_FILE_WRITE_TABLE;
        for(uint32_t i=0;i< this->col_num;i++)
            out_file<< this->header[i]<<",";
        out_file<<"\n";
        for(uint32_t i=0;i< this->col_num;i++)
            out_file<< this->col[this->header[i]]<<",";
        out_file<<"\n";
        for(uint32_t i=0;i<this->records_num;i++){
            std::string new_line("");
            for(auto &p:this->header){
                new_line.append(this->vars[p][i]);
                new_line.push_back(',');
            }
            new_line.push_back('\n');
            out_file<<new_line;
        }
        return WRITE_SUCCESS;
    }
    void Table::remove_file(){
        std::string file(this->path);
        file.append(this->table_name);
        file.append(".csv");
        remove(file.c_str());

    }
    ReadDatabase Database::read() {
        std::string filename(this->path);
        filename.append(this->db_name);
        filename.append(".db");
        std::fstream in_file;
        in_file.open(filename,std::ios::in);
        std::string buffer;
        Table p;
        while(in_file>>buffer){
            p.table_name=buffer;
            p.db= this->db_name;
            p.read();
            this->tables.push_back(p);
            this->tables_nums++;
        }
        return READ_DATABASE_SUCCESS;
    }
    WriteDatabase Database::write() {
        std::string filename(this->path);
        filename.append(this->db_name);
        filename.append(".db");
        std::fstream out_file;
        out_file.open(filename,std::ios::out);
        if(!out_file.is_open())return FAILED_TO_OPEN_FILE_DB_OUT;
        out_file.flush();
        for(auto i=0;i<this->tables_nums;i++){
            out_file<< this->tables[i].table_name<<"\n";
        }
        return WRITE_DATABASE_SUCCESS;
    }
    ReadDatabase Database::load(){
        for(auto &p:this->tables)
            p.read();
        return READ_DATABASE_SUCCESS;
    }
    void Database::remove_file(){
        std::string file(this->path);
        file.append(this->db_name);
        file.append(".db");
        remove(file.c_str());

    }
}
