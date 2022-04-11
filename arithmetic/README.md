# 逆波兰式四则运算

由于输入中有浮点数和字符两种数据类型，所以用字符串读入原始表达式。

一个表达式中基本元素只有浮点数和操作符号需要一种数据结构将两种不同的元素储存在一起。

字符串符合要求。

````c++
std::vector<std::string> RevPolish;//逆波兰表达式
std::vector<std::string> nums;//分割后的表达式
````

首先读入表达式，再对字符串进行分割，将每个数字和符号单独的存放在字符串数组当中。

````c++
for(auto const &p:Origin){	//Origin是原始输入
        if(ch.contains(p)){//ch是一个包含+，-，*，/，(，)的集合，contains是c++20的新特性，此步骤用来判断是否为字符
            if(!numStr.empty()){		//如果是字符且临时字符串不为空，说明上一个数字已经结束
                nums.push_back(numStr);//将数字存入
                numStr.clear();			//清除临时字符串
                numStr.push_back(p);	
                nums.push_back(numStr);//	以字符串形式存入运算符号
                numStr.clear();
            } else{
                numStr.push_back(p); //如果临时为空则直接将运算符号存入
                nums.emplace_back(numStr);
                numStr.clear();
                continue;
            }

        } else{
            numStr.push_back(p);//如果不是存入临时字符串中
        }

    }//得到分割好的表达式nums
````

逆波兰表达式的计算需要栈，可用std::vector实现

### 转换表达式，由中缀转后缀：

​	**规则：**从左到右遍历中缀表达式的每个数字和符号，若是数字就输出，即成为后缀表达式的一部分；若是符号，则判断其与栈顶符号的优先级，若是右括号或者优先级不高于栈顶符号则栈顶元素依次出栈并输出，并将当前符号进栈，一直到最终输出后缀表达式为止。

````mermaid
graph TD
a["当前元素"]-->A{"是否为符号"}
A-->|是|B{"是否为右括号"}
A-->|否|C["进入表达式"]
B-->|是|E["栈顶元素依次进入<br>表达式直至左括号"]
B-->|否|F{"优先级是否高于<br>栈顶元素"}
F-->|是|G["栈顶元素依次出栈并输出<br>直至栈空"]
F-->|否|H[当前元素进栈]
G-->H
E-->I
C-->I
H-->I["下一个元素"]
I-->|直到输出后缀表达式|a
````

**代码：**

````c++
    std::vector<std::string> temp;
    for(auto &p:nums){
        if (str.contains(p)){//如果是符号
            if (p == ")") {//如果是右括号
                while (temp.back() != "(") {
                    RevPolish.push_back(temp.back());
                    temp.pop_back();
                }
                temp.pop_back();
            } else if ((!temp.empty()) && (temp.back() == "*" || temp.back() == "/") && (p == "+" || p == "-")) 			{//如果优先级高于栈顶
                while (!temp.empty())   {
                    RevPolish.push_back(temp.back());
                    temp.pop_back();
                }
                temp.push_back(p);
            } else {//其余情况
                temp.push_back(p);
            }
        } else{//如果是数字
            RevPolish.push_back(p);
        }
    }
````





### **计算**

计算相对就简单了

**规则：**

从左到右遍历表达式的每个数字和符号，如果是数字就进栈，如果是符号将栈顶的元素两个元素出栈，进行该运算，运算结果进栈

````mermaid
graph TD
a["当前元素P"]-->A{"是否为符号 "}
A-->|否|B["当前元素进栈"]
A-->|是|C["栈顶两个元素a,b元素出栈"]
C-->D["c=bPa"]
D-->E["c入栈"]
F["下一个元素"]
B-->F
E-->F
F-->a
````

**代码：**

````c++
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
````

结束。
