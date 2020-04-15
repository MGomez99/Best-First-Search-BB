#include <iostream>
#include <cstdlib>
#include <vector>
#include <queue>
#include <algorithm>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

//"item" struct to store all respective values for a given item i*/
typedef struct item {
    int id; //item number before sorting (essentially relates to order of items created)
    int profit; //profit attributed to item
    int weight; //weight attributed to item
    float ppw; //price-per-unit-weight ratio
    string s; //string of 'weight,profit'
    // constructors
    item(int i, int w, int p){
        id = i;
        profit = p;
        weight = w;
        ppw = float(p)/float(w);
        s = to_string(w)+","+to_string(p);
    }
    item(){
        id = -1;
        profit = -1;
        weight = -1;
        s= "";
    }
    //overload < operator for sorting by profit-per-unit-weight
    bool operator<(const item& r) const {
        return ppw > r.ppw;
    }
} item;
// "Node"" struct for creating our tree
// Each Node will be put in our priority queue
typedef struct Node{
    int totalItems; // total items added thus far
    int level; // total items considered == level
    int weight; // total weight 
    float bound; // bound obtained from the fractional knapsack problem
    int profit; // total profit
    vector<item> included; // all items included
    // constructors
    Node(int w, int p, int lvl, int total){
        weight = w;
        profit = p;
        level = lvl;
        included = {};
        totalItems = total;
        bound = -1;
    }
    Node(){
        totalItems = -1;
        level = -1;
        weight = -1;
        bound = -1;
        profit = -1;
        included = {};
    }
    // copies items from previous vector to "included" vector
    // used instead of copy constructor
    // essentially will store all previous items of the parent node, then either add an item or not dependsing on if it's a "no" or "yes" child
    void copy_items(vector<item> v){
        vector<item> new_(v);
        included = new_;
    }
    
} Node;

/*Global Vars*/
vector<item> items; 
vector<item> solution_set;
int visited_nodes;
int visited_leafs;
int solution_size;

/*Utility Functions*/
//calculates upper bound for 0-1 knapsack by using a candidate from the fractional knapsack variation
float bound(Node n, int capacity, int max_items){
    int weight = n.weight;
    float bound = float(n.profit);
    int i = n.level;
    while((weight < capacity) && (i+1<=max_items)){ //while we're not over capacity and there are more items to consider
        if(weight + items[i].weight <= capacity){ //if we can add the next item
            //include full item i
            weight += items[i].weight;
            bound +=  float(items[i].profit);
        }
        else{
            float difference = float(capacity-weight);
            float fract = difference/float(items[i].weight); //add fraction of item          
            weight = capacity; //we've now filled up the fractional knapsack
            bound +=  float(items[i].profit)* fract;     
        }
        i += 1;
    }
    return bound;
}

//comparator for priority queue ordering
struct cmpr{
    bool operator() (const Node& l, const Node& r) const{
        return l.bound <  r.bound;
    }
};

// for reading in from file
// assumes correct format
void getInput(string filename, int& problemsize, int& weightcap, vector<item>& it){
    ifstream infile(filename);
    
    
    vector<vector<int>> data;
    
    while (infile.good()){
        string line;
        if(!getline(infile, line)) break;
        istringstream ss(line);
        vector<int> record;
        while(ss){
            string str;
            getline(ss, str, ',');
            stringstream number_as_string(str);
            int x = 0;
            number_as_string >> x;
            record.push_back(x);        
        }
        data.push_back(record);
    }
    // parse input from file 
    problemsize = data[0][0];
    weightcap = data[0][1];
    int id_count = 1;
    for(int i = 1; i < data.size(); i++){
        it.push_back(item(id_count, data[i][0], data[i][1]));
        id_count++;
    }
}
