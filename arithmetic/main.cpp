#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <set>
void toReversePolish(std::vector<char> &s);
void  Calculate(std::vector<char> &s);
#pragma Optimize(2)
int main() {
    std::string Origin;
    std::string numStr;
    std::set<char> const ch {'+','-','*','/','(',')'};
    std::set<float> const chf {(float)'+',(float)'-',(float)'*',(float)'/',(float)'(',(float)')'};
    std::cin>>Origin;

    std::vector<std::pair<float,bool>> nums;
    std::vector<std::pair<float,bool>> RevPolish;
    float num;
    for(auto const &p:Origin){
        if(ch.contains(p)){

            if(!numStr.empty()){
                num=std::stof(numStr);
                nums.emplace_back(num,false);
                nums.emplace_back((float)p,true);
                numStr.clear();
            } else
            {
                nums.emplace_back((float)p,true);
                continue;
            }

        } else{
            numStr.push_back(p);
        }

    }
    if(!numStr.empty()) {
        num = std::stof(numStr);
        nums.emplace_back(num, false);
    }
    std::stack<std::pair<float,bool>> temp;
    for(auto &p:nums){
        if(p.second){
            if(p.first==(float)')'){
                while (temp.top().first!=(float )'('){
                    RevPolish.emplace_back(temp.top());
                    temp.pop();
                }
                temp.pop();
            }else if((!temp.empty())&&(temp.top().first==(float)'*'||temp.top().first==(float)'/')&&(p.first==(float)'+'||p.first==(float)'-')){
                while (!temp.empty()){
                    RevPolish.emplace_back(temp.top());
                    temp.pop();
                }
                temp.push(p);
            } else{
                temp.push(p);
            }
        }else{
            RevPolish.emplace_back(p);
        }
    }
    while (!temp.empty()){
        RevPolish.emplace_back(temp.top());
        temp.pop();
    }
    float x;
    float opA;
    float opB;
    for(auto &p:RevPolish){
        if(p.second){
            opA=temp.top().first;
            temp.pop();
            opB=temp.top().first;
            temp.pop();
            if(p.first==(float )'-')
                x=opB-opA;
            else if(p.first==(float )'+')
                x=opB+opA;
            else if(p.first==(float )'*')
                x=opB*opA;
            else if(p.first==(float )'/')
                x=opB/opA;
            temp.push(std::make_pair(x,true));

        } else{
            temp.push(p);
        }
    }

    float res =temp.top().first;
    std::cout<<res;

}
