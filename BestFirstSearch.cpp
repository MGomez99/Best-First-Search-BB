#include "BestFirstSearch.h"
int BFSBB(int capacity, int numitems){
    visited_nodes = 0;
    visited_leafs = 0;
    //assume items[] is sorted by profit per unit weight

    //initialize priority queue
    vector<Node> q; 

    Node v;
    int maxprofit = 0;
    //set v to root of the tree
    v.level = 0;
    v.profit = 0;
    v.weight = 0;
    v.bound = bound(v, capacity, numitems);
    v.totalItems = 0;

    q.push_back(v); // add to priority queue
    while(!q.empty()){
        // get priorty element (best bound)
        v = q.front();
        //cout<<"(LEVEL = "<<v.level <<")"<<"\t\tprofit = "<<v.profit <<" weight = "<<v.weight <<" bound = " << v.bound<< " items added = " << v.totalItems<< endl;
        q.erase(q.begin());

        visited_nodes++;
        // case : solution can not expand any farther (no more items to consider)
        if (v.level == numitems){
            visited_leafs++;
            make_heap(q.begin(), q.end(), cmpr()); // update priority queue
            continue;

        }
        // case : solution not feasible
        else if (v.weight > capacity){
            visited_leafs++;
            make_heap(q.begin(), q.end(), cmpr()); // update priority queue
            continue;

        }
        // case : node is promising
        else if(v.bound > maxprofit){
            // expand current node
            // two choices

            // Choice 1: "yes" child (we add the next item)
            Node u(v.weight + items[v.level].weight, v.profit + items[v.level].profit , v.level + 1, v.totalItems + 1);
            u.copy_items(v.included); // keep track of solution by copying the parent's list of added items
             // u adds current item being considered
            u.included.push_back(items[v.level]);
            u.bound = bound(u, capacity, numitems); // calculate bound
            if((u.weight <= capacity) && (u.profit > maxprofit)){
                maxprofit = u.profit;
                //cout << "\t MAX PROFIT NOW " << to_string(maxprofit)<<endl;
                solution_set = u.included; //current best solution
                solution_size = u.totalItems;
            }
            
            q.push_back(u);
            

            // Choice 2: "no" child (we don't add the next item)
            Node w(v.weight, v.profit, v.level + 1, v.totalItems);
            w.copy_items(v.included); // keep track of solution by copying the parent's list of added items
            // w skips adding current item being considered (unlike u)
            w.bound = bound(w, capacity, numitems); // calculate bound
            
            q.push_back(w);
            
        }
        // case: node is not promising
        else{
            
            visited_leafs++;
        }
        // make sure priority queue is updated
        make_heap(q.begin(), q.end(), cmpr()); // update priority queue

    }
    // priority queue is empty, we have our solution
    return maxprofit;
}

int main(int argc, char** argv){
    //Get input strings
    if(argc < 3){
        cout << "usage:  BestFirstSearch <input.txt> <output.txt> " <<endl;
        return 0;
    }
    string inputfile = argv[1];
    string outputfile = argv[2];


    // initiallize variables (see header file for declarations)
    items = {};
    solution_set = {};
    solution_size = 0;

    // import problem
    int sizeofproblem = 0;
    int weightcap = 0;
    getInput(inputfile, sizeofproblem, weightcap, items);

    // sort by decreasing profit-per-unit-weight
    sort(items.begin(), items.end());

    for(int i = 0; i < items.size(); i++) cout << "item" << items[i].id<<" "<<items[i].s << "(ratio = " << items[i].ppw << "); ";
    cout<<endl;

    // get the optimal solution, other specifics of the solution are stored in global variables
    int sol = BFSBB(weightcap, sizeofproblem);
    
    //print solution to file
    ofstream fp;
    fp.open(outputfile);

    fp << sizeofproblem<<","<<sol <<"," <<  solution_size<< endl;
    fp << visited_nodes<<","<<visited_leafs << endl;
    for (auto it = solution_set.begin(); it != solution_set.end(); it++)
        fp << (*it).s <<endl;;
    fp.close();
}