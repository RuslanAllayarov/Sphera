#include <iostream>
#include <string>
#include <stdexcept>
#include <cctype>


class Calc{
    const std::string exp;
    int result;
public:
    Calc(){}
    Calc(const std::string& str)
        : exp(str), result(0) {}
    void print(){
        std::cout << result << std::endl;
    }
    std::string SkipTab(const std::string &str)
    {
        std::string res;
        for (int i = 0; i < str.size(); i++)
        {
            if(str[i] == ' '){}
            else {res += str[i];}
        }
        return res;
    }
    std::string Correct(const std::string& str)
    {
        std::string res;
        std::string gstr;
        if ((str[0] == '*') || (str[0] == '/') || (!isdigit(str[str.size() - 1])))
        {
            throw std::runtime_error("");
        }
        else
        {
            for (int i = 0; i < str.size(); i++)
            {
                int sign = 1;
                int flag = 0;
                while(str[i] == '-'){
                    sign *= -1; 
                    i++;
                    flag = 1;
                }
                if (flag == 1)
                {
                    if (sign == 1){res += '+';}
                    else {res += '-';}
                    i --;
                }
                else
                {
                    res += str[i];
                }
            }
            for (int i = 0; i < res.size();i++)
            {
                if (res[i] == '+')
                {
                    if ((res[i+1] == '+') && (i + 1 < res.size())){
                        gstr += '+';
                        i++;
                    }
                    else if ((res[i+1] == '-') && (i + 1 < res.size())){
                        gstr += '-';
                        i++;
                    }
                    else{gstr += '+';}
                }
                else {gstr += res[i];}
            }
        }
        return gstr;
    }

    std::string UnMinus(const std::string& str)
    {
        std::string res = str;
        size_t Len = str.size();
        for (int i = 0; i < Len; i++)
        {
            if ((str[i] == '*') || (str[i] == '/'))
            {
                int pos;
                int j = i - 1;
                while((j > 0) && (isdigit(str[j])))
                {
                    j--;
                }
                if (j == 0){pos = 0;}
                else{pos = j + 1;}
                i++;
                for (; (i < Len) && (str[i] == '-');i++)
                {
                    res.erase(i, 1);
                    res.insert(pos, "-");
                }
                
                while ((str[i] != '+') && (i < Len) && (str[i] != '-'))
                {
                    if ((str[i] == '*') || (str[i] == '/'))
                    {
                        i++;
                        for (; (i < Len) && (str[i] == '-');i++)
                        {
                            res.erase(i, 1);
                            res.insert(pos, "-");
                        }
                    }
                    i++;
                }
            }
        }
        return res;
    }

    std::string ParseMultDiv(std::string &str)
    {
        std::string rstr;
        size_t len = str.size();
        for (int i = 0; i < len; i++)
        {
            int flag = 0;
            if ((str[i] == '*') || (str[i] == '/'))
            {
                char op = str[i];
                if (i == 0){
                    throw std::runtime_error("");
                }
                std::string left;
                std::string right;
                int j = i - 1;
                while((j > 0) && (isdigit(str[j]))){
                    j--;
                }
                if (j == 0){
                    left = str.substr(0, i);
                }
                else{
                    left = str.substr(j + 1, i - j - 1);
                }
                std::int64_t res = stoi(left);
                int ind_first = i;
                size_t lSize = left.size();
                if (i == lSize)                
                {
                    rstr = "";
                }
                else
                {
                    rstr = rstr.substr(0, rstr.size() - lSize);
                }
                i++;
                while ((i < len) && (str[i] != '+') && (str[i] != '-'))
		{
                    if ((str[i] == '*') || (str[i] == '/'))
		    {
                        right = str.substr(ind_first + 1, i - ind_first - 1);
                        if (op == '*')
			{
                            res *= stoi(right);
                        }
                        else{
                            if (stoi(right) == 0)
			    {
                                throw std::runtime_error("Dividing by zero");
                            }
                            res /= stoi(right);
                        }
                    op = str[i];
                    ind_first = i;
                    }
                    i++;
                }
                if (op == '*')
		{
                    res *= stoi(str.substr(ind_first + 1, i - ind_first - 1));
                }
                else{
                    if (stoi(str.substr(ind_first + 1, i - ind_first - 1)) == 0){
                        throw std::runtime_error("Dividing by zero");
                    }
                    res /= stoi(str.substr(ind_first + 1, i - ind_first - 1));
                }
                rstr += std::to_string(res);
                flag = 1;
            }
            else{
                rstr += str[i];
            }
            if (flag) {i--;}
        }
        return rstr;
    }

    void ParseAddSum(const std::string& str)
    {
        std::string rstr = str;
        size_t pos;
        result = stoi(rstr, &pos);
        while (pos < rstr.size())
        {
            rstr = rstr.substr(pos);
            result += stoi(rstr, &pos);
        }  
    }
    
    void Res()
    {
        std::string remainstr = exp;
        remainstr = SkipTab(remainstr);
        remainstr = UnMinus(remainstr);
        remainstr = Correct(remainstr);
        remainstr = ParseMultDiv(remainstr);
        ParseAddSum(remainstr);
        print();
    }
};


int main(int argc, char* argv[])
{
    if (argc !=2){
        std::cout << "error" << std::endl;
        return 1;
    }
    try{
        std::string str = argv[1];
        Calc expr(str);
        expr.Res();
        return 0;
    }
    catch (std::runtime_error&)
    {
        std::cout << "error";
        return 1;
    }
}
