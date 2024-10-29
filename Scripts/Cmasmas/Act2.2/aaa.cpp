#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <unordered_map>

using namespace std;

string getString(string input_str) {
    
    int visited[256] = {0};
    bool ya[256] = {false};
    stack<char> todos;
    string res = "";
    
    for (int x=0; x<input_str.size();x++) {visited[input_str[x]]++;}
    
    for (int x=0; x<input_str.size();x++) {
        
        visited[input_str[x]]--;

        if (!ya[input_str[x]]){
            while (!todos.empty() && todos.top() < input_str[x] && visited[todos.top()] > 0) {
                ya[todos.top()] = false;
                todos.pop();
            }
            todos.push(input_str[x]);
            ya[input_str[x]] = true;
        }
    }
    while (!todos.empty()) {
        res = todos.top() + res;
        todos.pop();
    }

    return res;
}

int main() {
    string input_str;
    cin >> input_str;
    cout << getString(input_str) << endl;
    return 0;
}
