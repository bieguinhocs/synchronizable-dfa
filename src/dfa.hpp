#ifndef DFA_HPP
#define DFA_HPP

#include <bits/stdc++.h>
using namespace std;

class DFA {
    string input;
    int number_states;
    int initial_state;
    int number_final_states;
    vector<string> states;
    vector<string> pairs_states;
    unordered_set<string> non_final_states;
    unordered_set<string> final_states;
    map<pair<string, int>, string> transitions;

    void build_dfa();
    void add_final_state(const string& fs);
    void add_transition(const string& p, int a, const string& q);

    /* min sync private methods */
    //DFA* getB(DFA origin);
    vector<string> power_set();
    map<string, list<string>> get_adj_dec(DFA& dec_dfa);
    map<string, list<string>> get_adjB();
    vector<string> bfs();

    /* dec sync private methods */
    vector<string> generate_pairs_states();
    DFA create_dec_dfa();
    bool is_unit(const string& state);
    void dec_bfs(const string& pair_state, DFA& dec_dfa, int& count);

public:
    DFA();
    DFA(const string& input);
    void print_dfa();
    void print_sets();
    unordered_set<string> get_nfs();
    unordered_set<string> get_fs();
    map<pair<string, int>, string> get_transitions();
    void is_sync();
    string dec_sync();
    
};

DFA::DFA() = default;

DFA::DFA(const string& input) : input(input) {
    build_dfa();
}

void DFA::build_dfa() {   
    fstream file("../input/" + input + ".txt", ios::in);

    /* first line */
    file >> number_states >> initial_state >> number_final_states;
    int fs;
    for (size_t i = 0; i < number_final_states; ++i) {   
        file >> fs;
        add_final_state(to_string(fs));
    }
    for (size_t i = 0; i < number_states; ++i) {
        string state = to_string(i);
        if (final_states.find(state) == final_states.end())
            non_final_states.insert(state);
    }
    
    /* transitions */
    int p, a, q;
    for (size_t i = 0; i < 2 * number_states; ++i) {
        file >> p >> a >> q;
        add_transition(to_string(p), a, to_string(q));
    }

    for (size_t i = 0; i < number_states; ++i) {
        string state = to_string(i);
        states.push_back(state);
    }
}

void DFA::add_final_state(const string& fs) {
    final_states.insert(fs);
}

void DFA::add_transition(const string& p, int a, const string& q) {
    transitions[make_pair(p, a)] = q;
}

void DFA::print_dfa() {
    //cout << number_states << " " << initial_state << " " << number_final_states << " ";
    //for (const auto& fs : final_states)
    //    cout << fs << " ";
    //cout << endl;
    cout << "States: ";
    for (const auto& s : states)
        cout << s << " ";
    cout << endl;
    for (auto it = transitions.cbegin(); it != transitions.cend(); ++it)
        cout << it->first.first << " " << it->first.second << " " << it->second << endl;
    cout << endl;
    
}

void DFA::print_sets() {
    cout << "Non final states: ";
    for (auto it = non_final_states.cbegin(); it != non_final_states.cend(); ++it)
        cout << *it << " ";
    cout << endl;
    cout << "Final states    : ";
    for (auto it = final_states.cbegin(); it != final_states.cend(); ++it)
        cout << *it << " ";
    cout << endl;
    cout << "Power Set: ";
    for (const auto s : power_set()) {
        cout << s << " ";
    }
    cout << endl;
}

unordered_set<string> DFA::get_nfs() {
    return non_final_states;
}

unordered_set<string> DFA::get_fs() {
    return final_states;
}

map<pair<string, int>, string> DFA::get_transitions() {
    return transitions;
}

vector<string> DFA::power_set() {
    // O(2^n) algorithm
    vector<string> pset;
    unsigned int size = pow(2, number_states);
    int counter, i;

    for (counter = 0; counter < size; counter++) {
        string state;
        for (i = 0; i < number_states; i++) {           
            if (counter & (1 << i))
                state += states[i];           
        }
        pset.push_back(state);
    }
    return pset;
}

map<string, list<string>> DFA::get_adj_dec(DFA& dec_dfa) {
    unsigned int size = dec_dfa.states.size();
    map<string, list<string>> adj_list;

    for (const auto& s : dec_dfa.states) {
        list<string> l;
        string s_0 = dec_dfa.transitions[pair<string, int>(s, 0)];
        string s_1 = dec_dfa.transitions[pair<string, int>(s, 1)];
        l.push_back(s_0);
        if (s_0 != s_1)
            l.push_back(s_1);
        adj_list[s] = l;
    }
    return adj_list;
}

map<string, list<string>> DFA::get_adjB() {
    auto pset = power_set();
    unsigned int size = pset.size();
    map<string, list<string>> adj;
    // transitions
    return adj;
}

vector<string> DFA::bfs() {
    auto pset = power_set();
    unsigned int size = pset.size();
    string start = pset.back(); // bigger set of power set
    vector<string> path;

    // mark all states as not visited
    map<string, bool> visited;
    for (const auto& state : pset)
        visited[state] = false;
        
    // create a queue for bfs
    list<string> queue;

    // mark the current state as visited and enqueue it
    visited[start] = true;
    queue.push_back(start);

    list<string>::iterator it;
    while (!queue.empty()) {
        // dequeue a state from queue
        start = queue.front();
        path.push_back(start);
        queue.pop_front();
        
        /* get all adjacent states of the dequeued state "start"
           mark as visited and enqueue it if has not been visited */
        auto adj = get_adjB(); 
        for (it = adj[start].begin(); it != adj[start].end(); ++it) {
            if (!visited[*it]) {
                visited[*it] = true;
                queue.push_back(*it);
            }
        }
    }
    return path;
}

void DFA::is_sync() {
    auto path = bfs();
    for (const auto& state : path) {
        if (state.size() == 1) {
            // if unit state
            // print path
        }         
        else {
            cout << "NO" << endl;
        }               
    }
}

vector<string> DFA::generate_pairs_states() {
    vector<string> pairs_states;
    for (int i = 0; i < number_states; ++i) {
        for (int j = i + 1; j < number_states; ++j) {
            string pair = to_string(i) + to_string(j);
            pairs_states.push_back(pair);
        }
    }
    return pairs_states;
}

DFA DFA::create_dec_dfa() {
    DFA result;
    vector<string> pairs_states = generate_pairs_states();
    result.pairs_states = pairs_states;
    result.number_states = this->number_states + pairs_states.size();
    result.initial_state = this->initial_state;
    result.number_final_states = this->number_final_states;

    /* save states of new dfa */
    result.states.insert(result.states.end(), this->states.begin(), this->states.end());
    result.states.insert(result.states.end(), pairs_states.begin(), pairs_states.end());

    /* generate new transitions */
    result.transitions = this->transitions;
    for (const auto& p : pairs_states) {
        string s1 {p[0]};
        string s2 {p[1]};

        /* new transition with '0' */
        string s1_0 = this->transitions[pair<string, int>(s1, 0)];
        string s2_0 = this->transitions[pair<string, int>(s2, 0)];
        string s3_0 {s1_0};
        if (s1_0 != s2_0)
            s3_0 += s2_0;

        /* new transition with '1' */
        string s1_1 = this->transitions[pair<string, int>(s1, 1)];
        string s2_1 = this->transitions[pair<string, int>(s2, 1)];
        string s3_1 {s1_1};
        if (s1_1 != s2_1)
            s3_1 += s2_1;

        result.add_transition(p, 0, s3_0);
        result.add_transition(p, 1, s3_1);
    }
    return result;
}

bool DFA::is_unit(const string& state) {
    return state.size() == 1;
}

void DFA::dec_bfs(const string& pair_state, DFA& dec_dfa, int& count) {
    string start = pair_state;
    //vector<string> path;

    /* mark all states as not visited */
    map<string, bool> visited;
    for (const auto& state : dec_dfa.states)
        visited[state] = false;
        
    /* create a queue for bfs */
    list<string> queue;

    /* mark the current state as visited and enqueue it */
    visited[start] = true;
    queue.push_back(start);

    list<string>::iterator it;
    while (!queue.empty()) {
        /* dequeue a state from queue */
        start = queue.front();
        //path.push_back(start);
        queue.pop_front();
        
        /* get all adjacent states of the dequeued state "start"
           mark as visited and enqueue it if has not been visited */
        auto adj = get_adj_dec(dec_dfa); 
        for (it = adj[start].begin(); it != adj[start].end(); ++it) {
            if (!visited[*it]) {
                visited[*it] = true;
                queue.push_back(*it);
                if (is_unit(*it)) {
                    ++count;
                    queue.clear();
                    break;
                }
            }
        }
    }
    //return path;
}

string DFA::dec_sync() {
    string output;
    if (this->number_states == 1) {
        output = "SI";
        return output;
    }
    DFA dec_dfa = create_dec_dfa();
    auto pairs_states = dec_dfa.pairs_states;
    int n_pairs = pairs_states.size();
    int count = 0;
    for (const auto& p : pairs_states) {
        dec_bfs(p, dec_dfa, count);
    }
    output = (count == n_pairs) ? "SI" : "NO";
    return output;
}

#endif // DFA_HPP