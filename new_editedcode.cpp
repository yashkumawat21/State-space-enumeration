#include <bits/stdc++.h>
#include <chrono>
using namespace std::chrono;
using namespace std;

vector<pair<string,pair<string,string>> > transitions;
string null = "NULL";
string default_state = "";
string input_file;

bool check(string str, string substr)
{
    if(str.find(substr)!=string::npos)
        return true;
    return false;
}


string overallcondn(string &str,stack<string> &st,stack<string> &tempst,string &nstate){
   
                                                for (int k = str.length() - 2; str[k] != ' '; k--)
                                                {
                                                    nstate = str[k] + nstate;
                                                }
                                                string fcond = "";
                                                
                                                while (!st.empty())
                                                {
                                                    string w = st.top();
                                                    // cout<<w<<endl;
                                                    st.pop();
                                                    tempst.push(w);
                                                    fcond = fcond + "!" + w + " & ";
                                                }

                                                return fcond;
}


string cond(string s)
{
    string output = "";
    stack<char> st;
    for (int i = 0; i < s.length(); i++)
    {
        if (s[i] == '(')
        {
            st.push(s[i]);
            output = output + s[i];
        }
        else if (s[i] == ')')
        {
            st.pop();
            output = output + s[i];
        }
        else if (!st.empty())
        {
            output = output + s[i];
        }
    }
    return output;
}

string nested_type_function(string str)
{
    string str1 = "if";
    string str2 = "else";
    size_t found = str.find(str2);
    if (found != string::npos)
    {
        size_t found_if = str.find(str1);
        if (found_if != string::npos)
        {
            return "elif";
        }
        else
            return "else";
    }
    else
        return "if";
}


void query012(ifstream &f, string &str, int &countbegin, string &nst, int &j, int &l, stack<string> &st, string &temp,int q)
{
    // ifstream f(input_file);
    string nif;
    if(q==0 || q==1)
        nif = cond(str);
    string findbegin_netsed = str.substr(str.length() - 5, 5);
    if (findbegin_netsed == "begin")
    {
        countbegin++;
        getline(f, str);
        j++, l++;
        while (!check(str, nst) && !check(str, "end"))
        {
            getline(f, str);
            j++, l++;
        }
        if (check(str, nst))
        {
            string nstate = "";
            for (int k = str.length() - 2; str[k] != ' '; k--)
            {
                nstate = str[k] + nstate;
            }
            string fcond = "";
            stack<string> tempst;
            while (!st.empty())
            {
                string w = st.top();
                // cout << w << endl;
                st.pop();
                tempst.push(w);
                fcond = fcond + "!" + w + " & ";
            }
            // change here
            if(q==0 || q==1)
                fcond = fcond + nif;
            else
                fcond = fcond.substr(0, fcond.length() - 3);  // for query 2
            while (!tempst.empty())
            {
                st.push(tempst.top());
                tempst.pop();
            }
            if(q==0 || q==1)
                st.push(nif);
            // cout << temp << " ----> " << fcond << " ----> " << nstate << endl;
            transitions.push_back({fcond,{temp,nstate}});
        }
        while (!check(str, "end"))
        {
            getline(f, str);
            j++, l++;
        }
        if (check(str, "end"))
        {
            countbegin--;
        }
    }
    else
    {
        getline(f, str);
        j++, l++;
        if (check(str, nst))
        {
            string nstate = "";
            for (int k = str.length() - 2; str[k] != ' '; k--)
            {
                nstate = str[k] + nstate;
            }
            string fcond = "";
            stack<string> tempst;
            while (!st.empty())
            {
                string w = st.top();
                cout << w << endl;
                st.pop();
                tempst.push(w);
                fcond = fcond + "!" + w + " & ";
            }
            if(q==0 || q==1)
                fcond = fcond + nif;
            else
                fcond = fcond.substr(0, fcond.length() - 3);
            while (!tempst.empty())
            {
                st.push(tempst.top());
                tempst.pop();
            }
            if(q==0 || q==1)
                st.push(nif);
            // cout << temp << " ----> " << fcond << " ----> " << nstate << endl;
            transitions.push_back({fcond,{temp,nstate}});
        }
        else
        {
            getline(f, str);
            j++, l++;
            return;
        }
    }
}

void fsm(int line,string ess)
{
    int l = line;
    ifstream f(input_file);
    string str;
    string temp;
    string condition;
    stack<string> st;
    string nst = ess;
    int flag_nst = 1;
    int flag =0;
    bool bool_seq=false;
    for (int j = 1; j <= l; j++)
    {
        getline(f, str);
        l++;
        if (j > line)
        {
            string word = "";
            for (int i = 0; i < str.length(); i++)
            {
                if (word == "endcase" || bool_seq == true)
                {
                    return;
                    
                }
                if ((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= '0' && str[i] <= '9') || str[i] == '-' || str[i] == '.' || str[i] == '_' || str[i] == '\'' || (str[i] >= 'A' && str[i] <= 'Z'))
                {
                    word = word + str[i];
                    // cout << word << " " << temp << endl;
                    if (word == nst && flag == 1 && flag_nst == 1)
                    {
                        word = "";
                        int z = str.length() - 2;
                        string qwe = "";
                        while (str[z] != ' ')
                        {
                            qwe = str[z--] + qwe;
                        }
                        if (temp != "default")
                        {
                            // cout << temp << " ----> " << null << " ----> " << qwe << endl;
                            transitions.push_back({null,{temp,qwe}});
                        }
                        else
                        {
                            // cout << temp << " ---->" << qwe << endl;
                            default_state = qwe;
                        }
                    }
                    if (word == "else" && flag == 1 && str[i + 1] != ' ')
                    {
                        int countbegin = 0;
                        string findbegin = str.substr(str.length() - 5, 5);
                        getline(f, str);
                        j++, l++;
                        if (findbegin == "begin")
                        {
                            countbegin++;
                            while (!check(str,"end") && !check(str, nst) && !check(str,"if"))
                            {
                                getline(f, str);
                                j++, l++;
                            }
                            if (check(str,"end"))
                            {
                                word = "";
                                flag = 0;
                                break;

                            }
                            else if (check(str, nst))
                            {   stack<string> tempst;
                               string nstate;
                               string fcond = overallcondn(str,st,tempst,nstate);
                               fcond = fcond.substr(0, fcond.length() - 3);
                                // cout << temp << " ----> " << fcond << " ----> " << nstate << endl;
                                transitions.push_back({fcond,{temp,nstate}});
                                word = "";
                                flag = 0;
                                break;
                            }

                            else if (check(str,"if"))
                            {
                                stack<string> nested_stack;
                                while (countbegin != 0)
                                {
                                    if (check(str,"end"))
                                    {
                                        countbegin--;
                                        continue;
                                    }
                                    string val = nested_type_function(str); // 0 for if ,1 for else if, 2 for else
                                    if (val=="if")
                                    {
                                        query012(f, str, countbegin, nst, j, l, st, temp,0);
                                    }
                                    if (val=="elif")
                                    {
                                       query012(f, str, countbegin, nst, j, l, st, temp,1);
                                    }
                                    if (val=="else")
                                    {
                                        query012(f, str, countbegin, nst, j, l, st, temp,2);
                                    }
                                    getline(f, str);
                                    j++, l++;
                                }
                                word = "";
                                flag = 0;
                                break;
                            }
                           
                        }
                        else
                        {
                            if (check(str, nst))
                            {
                                stack<string> tempst;
                               string nstate;
                               string fcond = overallcondn(str,st,tempst,nstate);
                                fcond = fcond.substr(0, fcond.length() - 3);
                                // cout << temp << " ----> " << fcond << " ----> " << nstate << endl;
                                transitions.push_back({fcond,{temp,nstate}});
                            }
                            else
                            {
                                word = "";
                                flag = 0;
                                break;
                            }
                        }
                        word = "";
                        flag = 0;
                        break;
                    }
                }
                else if (str[i] == ':')
                {
                    while (!st.empty())
                    {
                        st.pop();
                    }
                    flag = 1;
                    if (word != "")
                    {
                        temp = word;
                    }
                    word = "";
                }
                else if (word == "if" && flag == 1)
                {
                    int countbegin = 0;
                    condition = cond(str);
                    string findbegin = str.substr(str.length() - 5, 5);
                    getline(f, str);
                    j++, l++;
                    if(check(str,"<=")){
                        bool_seq = true;
                        break;
                    }
                    if (findbegin == "begin")
                    {
                        countbegin++;
                        if (nst != "")
                        {
                            while (!check(str, nst) && !check(str,"if"))
                            {
                                getline(f, str);
                                j++, l++;
                            }
                        }
                        if (check(str,"if"))
                        {
                            string findbegin2 = str.substr(str.length() - 5, 5);
                            if (findbegin2 == "begin")
                                countbegin++;
                            string ifcond = cond(str);
                            getline(f, str);
                            j++, l++;
                            while (!check(str, nst))
                            {
                                getline(f, str);
                                j++, l++;
                            }
                            string nstate = "";
                            int k = str.length() - 2;
                            while (str[k] != ' ')
                            {
                                nstate = str[k--] + nstate;
                            }
                            if (flag_nst == 0)
                            {
                                flag_nst = 1;
                                k -= 3;
                                while (str[k] != ' ' && str[k]==' ')
                                {
                                    nst = str[k--] + nst;
                                }
                            }
                            // cout << temp << " ----> " << condition << " & " << ifcond << " ----> " << nstate << endl;
                            string inn=condition + " & " + ifcond;
                            transitions.push_back({inn,{temp, nstate}});
                            getline(f, str);
                            j++, l++;
                            if (check(str,"end"))
                                countbegin--;
                            if (countbegin == 0)
                            {
                                st.push(condition);
                                word = "";
                                break;
                            }
                            getline(f, str);
                            j++, l++;
                            if (check(str,"end"))
                                countbegin--;
                            if (countbegin == 0)
                            {
                                st.push(condition);
                                word = "";
                                break;
                            }
                            while (!check(str, nst))
                            {
                                getline(f, str);
                                j++, l++;
                            }
                            nstate = "";
                            k = str.length() - 2;
                            while (str[k] != ' ')
                            {
                                nstate = str[k--] + nstate;
                            }
                            // cout << temp << " ----> " << condition << " & "
                            //      << "! " << ifcond << " ----> " << nstate << endl;
                            transitions.push_back({condition + " & " + "! " + ifcond,{temp,nstate}});
                        }
                        else
                        {
                            if (flag_nst == 1)
                            {
                                while (!check(str, nst))
                                {
                                    getline(f, str);
                                    j++, l++;
                                }
                            }
                            string nstate = "";
                            int k = str.length() - 2;
                            while (str[k] != ' ')
                            {
                                nstate = str[k--] + nstate;
                            }
                            if (flag_nst == 0)
                            {
                                flag_nst = 1;
                                k -= 3;
                                while (str[k] != ' ' && str[k] != '	')
                                {
                                    nst = str[k--] + nst;
                                }
                            }
                            // cout << temp << " ----> " << condition << " ----> " << nstate << endl;
                            transitions.push_back({condition,{temp,nstate}});
                        }
                        st.push(condition);
                        word = "";
                        break;
                    }
                    else
                    {
                        if (flag_nst == 1)
                        {
                            string line1state = "";
                            int k = 0;
                            while (str[k] == ' ' || str[k] == '	')
                                k++;
                            while (str[k] != ' ')
                            {
                                line1state += str[k++];
                            }
                            if (line1state == nst)
                            {
                                string nstate = "";
                                k = str.length() - 2;
                                while (str[k] != ' ')
                                {
                                    nstate = str[k--] + nstate;
                                }
                                // cout << temp << " ----> " << condition << " ----> " << nstate << endl;
                                transitions.push_back({condition,{temp,nstate}});
                            }
                            else
                                break;
                        }
                        else
                        {
                            string nstate = "";
                            int k = str.length() - 2;
                            while (str[k] != ' ')
                            {
                                nstate = str[k--] + nstate;
                            }
                            if (flag_nst == 0)
                            {
                                flag_nst = 1;
                                k -= 3;
                                while (str[k] != ' ' && str[k] != '	')
                                {
                                    nst = str[k--] + nst;
                                }
                            }
                            transitions.push_back({condition,{temp,nstate}});
                            // cout << temp << " ----> " << condition << " ----> " << nstate << endl;
                        }
                    }
                    st.push(condition);
                    word = "";
                    break;
                }
                else if (word == "else" && i + 2 < str.length() && str.substr(i + 1, 2) == "if" && flag == 1)
                {
                    string fcond = "";
                    i += 2;
                    condition = cond(str);
                    int countbegin = 0;
                    string findbegin = str.substr(str.length() - 5, 5);
                    getline(f, str);
                    j++, l++;
                    if(findbegin=="begin")
                    {
                        countbegin++;
                        while (!check(str,"end") && !check(str, nst) && !check(str,"if"))
                        {
                            getline(f, str);
                            j++, l++;
                        }
                        if (check(str,"end"))
                        {
                            word = "";
                            break;
                        }
                        else if(check(str,nst))
                        {
                            stack<string> tempst;
                               string nstate;
                               string fcond = overallcondn(str,st,tempst,nstate);
                            fcond = fcond + condition;
                            transitions.push_back({fcond,{temp,nstate}});
                            // cout << temp << " ----> " << fcond << " ----> " << nstate << endl;
                            while (!tempst.empty())
                            {
                                st.push(tempst.top());
                                tempst.pop();
                            }
                            st.push(condition);
                            word = "";
                            break;
                        }
                        else if (check(str,"if"))
                        {
                            stack<string> nested_stack;
                            while (countbegin != 0)
                            {
                                if (check(str,"end"))
                                {
                                    countbegin--;
                                    continue;
                                }
                                string val = nested_type_function(str); // 0 for if ,1 for else if, 2 for else
                                if (val=="if")
                                {
                                    string nif = cond(str);
                                    nested_stack.push(nif);
                                    string findbegin_netsed = str.substr(str.length() - 5, 5);
                                    if (findbegin_netsed == "begin")
                                    {
                                        countbegin++;
                                        getline(f, str);
                                        j++, l++;
                                        while (!check(str, nst) && !check(str,"end"))
                                        {
                                            getline(f, str);
                                            j++, l++;
                                        }
                                        if (check(str, nst))
                                        {
                                            stack<string> tempst;
                               string nstate;
                               string fcond = overallcondn(str,st,tempst,nstate);
                                            fcond = fcond + condition + " & " + nif;
                                            while (!tempst.empty())
                                            {
                                                st.push(tempst.top());
                                                tempst.pop();
                                            }
                                            // cout << temp << " ----> " << fcond << " ----> " << nstate << endl;
                                            transitions.push_back({fcond,{temp,nstate}});
                                        }
                                        while (!check(str,"end"))
                                        {
                                            getline(f, str);
                                            j++, l++;
                                        }
                                        if (check(str,"end"))
                                        {
                                            countbegin--;
                                            getline(f,str);
                                        }
                                    }
                                    else
                                    {
                                        getline(f, str);
                                        j++, l++;
                                        if (check(str, nst))
                                        {
                                            stack<string> tempst;
                               string nstate;
                               string fcond = overallcondn(str,st,tempst,nstate);
                                            fcond = fcond + condition + " & " + nif;
                                            while (!tempst.empty())
                                            {
                                                st.push(tempst.top());
                                                tempst.pop();
                                            }
                                            // cout << temp << " ----> " << fcond << " ----> " << nstate << endl;
                                            transitions.push_back({fcond,{temp,nstate}});
                                        }
                                        else
                                        {
                                            getline(f, str);
                                            j++, l++;
                                            continue;
                                        }
                                    }
                                }
                                if(val=="elif")
                                {
                                    string nif = cond(str);
                                    string findbegin_netsed = str.substr(str.length() - 5, 5);
                                    if (findbegin_netsed == "begin")
                                    {
                                        countbegin++;
                                        getline(f, str);
                                        j++, l++;
                                        while (!check(str, nst) && !check(str,"end"))
                                        {
                                            getline(f, str);
                                            j++, l++;
                                        }
                                        if (check(str, nst))
                                        {
                                            stack<string> tempst;
                                            string nstate;
                                            string fcond = overallcondn(str,st,tempst,nstate);
                                            fcond = fcond + condition+" & ";
                                            while (!tempst.empty())
                                            {
                                                st.push(tempst.top());
                                                tempst.pop();
                                            }

                                            // cout<<nested_stack.empty()<<endl;
                                            while (!nested_stack.empty())
                                            {
                                                string w = nested_stack.top();
                                                // cout<<w<<endl;
                                                nested_stack.pop();
                                                tempst.push(w);
                                                fcond = fcond + "!" + w + " & ";
                                            }
                                            fcond = fcond + nif;
                                            while (!tempst.empty())
                                            {
                                                nested_stack.push(tempst.top());
                                                tempst.pop();
                                            }
                                            
                                            nested_stack.push(nif);
                                            // cout << temp << " ----> " << fcond << " ----> " << nstate << endl;
                                            transitions.push_back({fcond,{temp,nstate}});
                                        }
                                        while (!check(str,"end"))
                                        {
                                            getline(f, str);
                                            j++, l++;
                                        }
                                        if (check(str,"end"))
                                        {
                                            countbegin--;
                                            getline(f,str);
                                        }
                                    }
                                    else
                                    {
                                        getline(f, str);
                                        j++, l++;
                                        if (check(str, nst))
                                        {
                                            stack<string> tempst;
                                            string nstate;
                                            string fcond = overallcondn(str,st,tempst,nstate);
                                            fcond = fcond + " & " + condition;
                                            while (!tempst.empty())
                                            {
                                                st.push(tempst.top());
                                                tempst.pop();
                                            }

                                            while (!nested_stack.empty())
                                            {
                                                string w = nested_stack.top();
                                                // cout<<w<<endl;
                                                nested_stack.pop();
                                                tempst.push(w);
                                                fcond = fcond + "!" + w + " & ";
                                            }
                                            fcond = fcond + " & " + nif;
                                            while (!tempst.empty())
                                            {
                                                nested_stack.push(tempst.top());
                                                tempst.pop();
                                            }
                                            
                                            nested_stack.push(nif);
                                            // cout << temp << " ----> " << fcond << " ----> " << nstate << endl;
                                            transitions.push_back({fcond,{temp,nstate}});
                                        }
                                        else
                                        {
                                            getline(f, str);
                                            j++, l++;
                                            continue;
                                        }
                                    }
                                }
                                if(val=="else")
                                {
                                    string findbegin_netsed = str.substr(str.length() - 5, 5);
                                    if (findbegin_netsed == "begin")
                                    {
                                        countbegin++;
                                        getline(f, str);
                                        j++, l++;
                                        while (!check(str, nst) && !check(str,"end"))
                                        {
                                            getline(f, str);
                                            j++, l++;
                                        }
                                        if (check(str, nst))
                                        {
                                           stack<string> tempst;
                                            string nstate;
                                            string fcond = overallcondn(str,st,tempst,nstate);
                                            fcond = fcond + condition +" & ";
                                            while (!tempst.empty())
                                            {
                                                st.push(tempst.top());
                                                tempst.pop();
                                            }
                                            while (!nested_stack.empty())
                                            {
                                                string w = nested_stack.top();
                                                // cout<<w<<endl;
                                                nested_stack.pop();
                                                tempst.push(w);
                                                fcond = fcond + "!" + w + " & ";
                                            }
                                            fcond = fcond.substr(0, fcond.length() - 3);
                                            while (!tempst.empty())
                                            {
                                                nested_stack.push(tempst.top());
                                                tempst.pop();
                                            }
                                            
                                            // cout << temp << " ----> " << fcond << " ----> " << nstate << endl;
                                            transitions.push_back({fcond,{temp,nstate}});
                                        }
                                        while (!check(str,"end"))
                                        {
                                            getline(f, str);
                                            j++, l++;
                                        }
                                        if (check(str,"end"))
                                        {
                                            countbegin--;
                                            getline(f,str);
                                        }
                                    }
                                    else
                                    {
                                        getline(f, str);
                                        j++, l++;
                                        if (check(str, nst))
                                        {
                                            stack<string> tempst;
                                            string nstate;
                                            string fcond = overallcondn(str,st,tempst,nstate);
                                            fcond = fcond + " & " + condition;
                                            while (!tempst.empty())
                                            {
                                                st.push(tempst.top());
                                                tempst.pop();
                                            }
                                            while (!nested_stack.empty())
                                            {
                                                string w = nested_stack.top();
                                                // cout<<w<<endl;
                                                nested_stack.pop();
                                                tempst.push(w);
                                                fcond = fcond + "!" + w + " & ";
                                            }
                                            fcond = fcond.substr(0, fcond.length() - 3);
                                            while (!tempst.empty())
                                            {
                                                nested_stack.push(tempst.top());
                                                tempst.pop();
                                            }
                                            
                                            // cout << temp << " ----> " << fcond << " ----> " << nstate << endl;
                                            transitions.push_back({fcond,{temp,nstate}});
                                        }
                                        else
                                        {
                                            getline(f, str);
                                            j++, l++;
                                            continue;
                                        }
                                    }
                                }
                            }
                        }
                        if(check(str,nst))
                        {
                            stack<string> tempst;
                                            string nstate;
                                            string fcond = overallcondn(str,st,tempst,nstate);
                            fcond = fcond + condition;
                            transitions.push_back({fcond,{temp,nstate}});
                            // cout << temp << " ----> " << fcond << " ----> " << nstate << endl;
                            while (!tempst.empty())
                            {
                                st.push(tempst.top());
                                tempst.pop();
                            }
                            st.push(condition);
                            word = "";
                            break;
                        }
                        else
                        {
                            word="";
                            st.push(condition);
                            break;
                        }
                        st.push(condition);
                        break;
                    }
                    else
                    {
                        if (check(str, nst))
                        {
                            stack<string> tempst;
                                            string nstate;
                                            string fcond = overallcondn(str,st,tempst,nstate);
                            fcond = fcond + condition;
                            transitions.push_back({fcond,{temp,nstate}});
                            // cout << temp << " ----> " << fcond << " ----> " << nstate << endl;
                            while (!tempst.empty())
                            {
                                st.push(tempst.top());
                                tempst.pop();
                            }
                            st.push(condition);
                            word = "";
                            break;
                        }
                        else
                        {
                            word = "";
                            st.push(condition);
                            break;
                        }
                    }
                    word = "";
                    st.push(condition);
                    break;
                }
                else if (word == "else" && flag == 1)
                {
                    int countbegin = 0;
                    string findbegin = str.substr(str.length() - 5, 5);
                    getline(f, str);
                    j++, l++;
                    if (findbegin == "begin")
                    {
                        countbegin++;
                        while (!check(str,"end") && !check(str, nst) && !check(str,"if"))
                        {
                            getline(f, str);
                            j++, l++;
                        }
                        if (check(str,"end"))
                        {
                            word = "";
                            flag = 0;
                            break;
                        }
                        else if (check(str, nst))
                        {
                            // cout<<nst<<endl;
                            stack<string> tempst;
                                            string nstate;
                                            string fcond = overallcondn(str,st,tempst,nstate);
                            fcond = fcond.substr(0, fcond.length() - 3);
                            // cout << temp << " ----> " << fcond << " ----> " << nstate << endl;
                            transitions.push_back({fcond,{temp,nstate}});
                            word = "";
                            flag = 0;
                            break;
                        }

                        else if (check(str,"if"))
                        {
                            stack<string> nested_stack;
                            while (countbegin != 0)
                            {
                                if (check(str,"end"))
                                {
                                    countbegin--;
                                    continue;
                                }
                                string val = nested_type_function(str); // 0 for if ,1 for else if, 2 for else
                                if (val=="if")
                                    {
                                        query012(f, str, countbegin, nst, j, l, st, temp,0);
                                    }
                                    if (val=="elif")
                                    {
                                       query012(f, str, countbegin, nst, j, l, st, temp,1);
                                    }
                                    if (val=="else")
                                    {
                                        query012(f, str, countbegin, nst, j, l, st, temp,2);
                                    }
                                getline(f, str);
                                j++, l++;
                            }
                            word = "";
                            flag = 0;
                            break;
                        }
                        if (check(str, nst))
                        {
                            stack<string> tempst;
                                            string nstate;
                                            string fcond = overallcondn(str,st,tempst,nstate);
                            fcond = fcond.substr(0, fcond.length() - 3);
                            // cout << temp << " ----> " << fcond << " ----> " << nstate << endl;
                            transitions.push_back({fcond,{temp,nstate}});
                        }
                        else
                        {
                            while (!st.empty())
                            {
                                st.pop();
                            }
                            word = "";
                            flag = 0;
                            break;
                        }
                    }
                    else
                    {
                        if (check(str, nst))
                        {
                            stack<string> tempst;
                                            string nstate;
                                            string fcond = overallcondn(str,st,tempst,nstate);
                            fcond = fcond.substr(0, fcond.length() - 3);
                            // cout << temp << " ----> " << fcond << " ----> " << nstate << endl;
                            transitions.push_back({fcond,{temp,nstate}});
                        }
                        else
                        {
                            word = "";
                            flag = 0;
                            break;
                        }
                    }
                    word = "";
                    flag = 0;
                    break;
                }
                else if (str[i] == ' ')
                {
                    if (flag == 0 && word != "")
                    {
                        // cout << word << " " << temp << endl;
                        temp = word;
                    }
                    word = "";
                }
                else if (str[i] == '/')
                {
                    break;
                }
            }
            if (word == "endcase")
            {
                return;
            }
        }
    }
}

int main()
{
    
    auto start = high_resolution_clock::now();
    cout << "Enter the name of the design : ";
    cin >> input_file;
    ifstream file(input_file);
    string str;
    int ff = 0;
    transitions.clear();
    int cnt = 1;

    set<string> inpst,outst;
    int input_ver=0;
    int output_ver=0;
    while (getline(file, str))
    {
        string word = "";
        for (int i = 0; i < str.length(); i++)
        {
            if (str[i] == '/')
            {
                break;
            }

            else if(word=="input")
            {
                int temp = i+1;
                int k=str.length()-2;
                string inp="";
                for(int g=k;g>=temp;g--)
                {
                    // if(str[g]==' ')
                    // {
                    //     inpst.insert(inp);
                    //     break;
                    // }

                    if(str[g]==',')
                    {    
                        inpst.insert(inp);
                        inp="";
                    }

                    else
                    {
                        inp= str[g]+inp;
                    }
                    
                }
                word = "";

                
            }
            else if(word=="output")
            {
                int temp = i+1;
                int k=str.length()-2;
                string inp="";
                for(int g=k;g>=temp;g--)
                {
                    // if(str[g]==' ')
                    // {
                    //     inpst.insert(inp);
                    //     break;
                    // }

                    if(str[g]==',')
                    {    
                        inpst.insert(inp);
                        inp="";
                    }

                    else
                    {
                        inp= str[g]+inp;
                    }
                    
                }
                word = "";
            }
            else if(word=="reg")
            {
                bool flag = false;
                // cout << str.substr(i+1) << "-->";
                int pos1 = str.find('[');
                int pos2 = str.find(']');
                int pos3 = str.find(':');
                // cout << pos1 << " " << pos2 << " ";
                int ff1 = 0,ff2=0;
                if(pos1!=-1 and pos2!=-1)
                {
                    string s1 = str.substr(pos1+1,pos3-pos1);
                    string s2 = str.substr(pos3+1,pos2-pos3);
                    if(isdigit(s1[0]) and isdigit(s2[0])){
                    ff1 = stoi(s1);
                    ff2 = stoi(s2);
                    }
                }
                ff += abs(ff1-ff2+1);
                // cout <<"ff : "<< ff << endl;
                word = "";

            }
            else if (word == "case")
            {
                string ess="";

                for(int k=i+1;k<str.length();k++){

                    if(str[k]==')')break;


                    ess+=str[k];
                }

                cout<<ess<<endl<<endl<<endl;
                fsm(cnt,ess);
                word = "";
            }
            else if (str[i] == ' ' || str[i] == '(' || str[i] == ';' || str[i] == ',')
            {
                if (word != "")
                {
                    word = "";
                }
            }

            else if (str[i] == ':' || (str[i] >= 'a' && str[i] <= 'z') || (str[i] >= '0' && str[i] <= '9') || str[i] == '-' || str[i] == '.' || str[i] == '_' || str[i] == '\'' || (str[i] >= 'A' && str[i] <= 'Z'))
            {
                word = word + str[i];
            }
        }
        word = "";
        cnt++;
    }

   

    ofstream myfile;
    myfile.open (input_file+"op");
    myfile <<  "*******************************************************************************************************************************" << endl << endl << 
    "                            Finite State Machine For " << input_file  << endl << endl
    << "********************************************************************************************************************************" << endl << endl;

    set<string> allstates;
    set<string> states_loops;
    int loops = 0;
    int trans= 0;
ofstream graph;
int cntt=1;
    map<string,int> m;
    graph.open("out_grph.txt");
    for (auto it : transitions)
    {
        pair<string,pair<string,string>> p= it;
        myfile <<(p.second).first<<"----->  "<<p.first<<" ------>>  "<< (p.second).second<<endl<<endl;
        graph<<"\" " <<(p.second).first<<"\""<<"->"<<"\""<<p.first<<"\""<<"->"<<"\" " <<(p.second).second<<"\" "<< "[ label="<< "\""<<cntt<< "\"]" << endl;
        cntt++;
        m[(p.second).first]++;
        if ((p.second).first == (p.second).second)
        {
            loops++;
            states_loops.insert((p.second).first);
        }
        else
            trans++;
        allstates.insert((p.second).first);
        allstates.insert((p.second).second);
    }

    myfile << endl<< endl;

    myfile<<"Pair of initial state and final state :- "<<endl;

    for (auto it : transitions)
    {
        pair<string,pair<string,string>> p= it;
        myfile << "( "<<((p.second).first) <<" , "<< ((p.second).second)<<" )"<< endl;
    }
    myfile << endl<< endl<<endl;

    int maxt=0;
    for (auto it : m)
    {
        pair<string,int> p=it;

        maxt=max(maxt,p.second);
    }

    myfile<<"Max Transitions :  "<<maxt<<endl<<endl;


    

    myfile << "Total no of states = " << allstates.size() << endl << "All states are :-  " << endl;
    for (auto it : allstates)
    {
        myfile << it << " , ";
    }
    myfile << endl << endl;

    myfile << "No of self loops = " << loops << endl;
    myfile << "No of transitions = " << trans + loops << endl << endl;
    myfile << "States having self loops :-  ";
    if (loops == 0)
        myfile << "No states have self loops";
    else
    {
        for (auto it : states_loops)
        {
            myfile << it << " , ";
        }
    }

    myfile<<endl<<endl;
    myfile<<"Max Transitions :  "<<maxt<<endl<<endl;


    myfile<<"No. of Inputs in the verilog code  :  "<<inpst.size()<< endl;
    myfile<<"Name of Inputs in the verilog code  :  ";
    for (auto it : inpst)
    {
        myfile << it << " , ";
    }
    myfile<<endl<<endl;
    
    myfile<<"No. of Outputs in the verilog code  :  "<<outst.size()<<endl;
    myfile<<"Name of Outputs in the verilog code  :  ";
    for (auto it : outst)
    {
        myfile << it << " , ";
    }
    myfile<<endl<<endl;
    
    myfile<<"No. of FlipFlop "<<ff;
    myfile << endl << endl;

    auto stop = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(stop - start);
    // auto duration = duration_cast<microseconds>(stop - start);
    myfile << endl;
    myfile << endl;
    myfile << "Time Taken By Model: " << time_span.count() << " s" << endl;

   
}