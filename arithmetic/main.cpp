#include <iostream>
#include <string>
#include <vector>
#include <set>
#pragma Optimize(2)
int main() {
    std::string Origin;
    std::string numStr;
    std::set<char> const ch {'+','-','*','/','(',')'};
    std::set<std::string> str{"+","-","*","/","(",")"};
    getline(std::cin,Origin);
    std::vector<std::string> nums;
    std::vector<std::string> RevPolish;
    for(auto const &p:Origin){
        if(ch.contains(p)){

            if(!numStr.empty()){
                nums.push_back(numStr);
                numStr.clear();
                numStr.push_back(p);
                nums.push_back(numStr);
                numStr.clear();
            } else{
                numStr.push_back(p);
                nums.emplace_back(numStr);
                numStr.clear();
                continue;
            }

        } else{
            numStr.push_back(p);
        }

    }
    if(!numStr.empty()) {
        nums.emplace_back(numStr);
    }
    std::vector<std::string> temp;
    for(auto &p:nums){
        if (str.contains(p)){
            if (p == ")") {
                while (temp.back() != "(") {
                    RevPolish.push_back(temp.back());
                    temp.pop_back();
                }
                temp.pop_back();
            } else if ((!temp.empty()) && (temp.back() == "*" || temp.back() == "/") && (p == "+" || p == "-")) {
                while (!temp.empty()) {
                    RevPolish.push_back(temp.back());
                    temp.pop_back();
                }
                temp.push_back(p);
            } else {
                temp.push_back(p);
            }
        } else{
            RevPolish.push_back(p);
        }
    }
    while (!temp.empty()){
        RevPolish.emplace_back(temp.back());
        temp.pop_back();
    }
    std::vector<float> res;
    float x;
    float opA;
    float opB;
    for(auto &p:RevPolish){
        if(str.contains(p)){
            opA= res.back();
            res.pop_back();
            opB= res.back();
            res.pop_back();
            if(p=="-")
                x=opB-opA;
            else if(p=="+")
                x=opB+opA;
            else if(p=="*")
                x=opB*opA;
            else if(p=="/"   )
                x=opB/opA;
            res.push_back(x);

        } else{
            x= stof(p);
            res.push_back(x);
        }
    }

    float ans=res.back();
    std::cout<<ans;

}
