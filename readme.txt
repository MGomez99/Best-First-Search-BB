COMPILATION:
	make or make all will compile the program
	make run does the command ./BestFirstSearch input.txt output.txt
	make clean deletes the executable BestFirstSearch and output.txt
	
	after running make, run the program by typing ./BestFirstSearch inputfile.txt outputfile.txt
	undefinied behavior if inputfile does not exist


DATA STRUCTURES:
- made item struct to store each item in a package, including attributes such as profit, weight, price-per-unit-weight, etc.
- made node struct to store the relevant information at every node in the state tree used for Branch and Bound, including level, items added, items, weight, bound, profit, etc.

- made comaprator struct for ordering items in priority queue with highest bound at the "top"
- there is no "Tree" per se, each Node is technically a aprt of the tree, but there was no need to make a tree data structure since each node contains all relevant information
- child nodes are only created if the aprent is considered, so the relationship between ndoes is not broken (can't skip consideration parents to get to a node)

- used vectors to store items added at each node, to store nodes (priortity queue), to store solution set
- used make_heap STL funciton to turn vector into priority queue in O(3N) time


ANALYSIS:
- this algorithm's optimizations compared to other algorithms lie in its use of Best-First-Search and Branch & Bound. Since Branch & Bound uses a state-tree, 
best first search is important to use at it only loks at the most promising nodes that have been queued (priority queue)
- The bound is obtained by using the fractional kapsack approach. The fractional knapsack solution will always be an upper bound, since it uses the same sorting as this algorithm, but it will always have a full
knapsack in the end. So, it is an upperbound. 
- Every level in the state tree represents a specific item being considered. We sort our items in order of decreasing profit-per-unit-weight. So at level 0, nodes (root) have no items considered.
At level 5, we have considered 5 items. Considering an item means we either add or didnt add the item. 
-In theory, we have a choice for every item (add or don't add). This means that the state tree has at maximum 2^N -1 nodes. If we were to consider each of these (in the vcase where all items can fit in the knapsack),
we make 2^N -1 decisions => the while() loop will run 2^N -1 times
- every time we make a new node, we copy the parent's vector. There are many ways that copying can occur, but for simplicity's sake, we'll say that copying is O(1) through a copy-on-write approach (sicne we don't alter previous nodes in the child)

- the make_heap algorithm has complexity O(N) since it has optimizations in choosing when to sift up vs sift down when building the heap (STL FUNCTION)
- we re-heapify every loop iteration

- front() and erase() are, for simplicity's sake, assumed to be O(1)
- If N is large, then in the worst case, we see 2^N - 1 iterations with each iteration reheapifying in O(ITEMS_CONSIDERED_AT_CURRENT_TIME) time with an insignificant amount of O(1) operations. 
- ITEMS_CONSIDERED_AT_CURRENT_TIME is N at the final level, so on average, it will be N/2 ~=~ N
- If N is small, then the O(1) operations dominate the loop iterations


CLASSES & INTERACTIONS:

 - vector of Items uses sorted() and the overlaoded method for the < operator to sort by decreasing PPW
 - vector of nodes / "priority queue" uses the comparator struct and make_heap to keep the priority value (highest bound) at the first index
 - getInput function reads from data and parses the input, making sure to use the expected commas as delimiters, and stores information into variables (oroblemsize, weightcap, items)
 - bound() function calculates an upper bound using the fractional knapsack approach, resulting in a bound that is equal
	+ in msot cases, the last item added will be a fraction of the whole item
 - BSFBB function is what actually does the algorithm
	+ the function assumes that it has a sorted list of items and first pushes a node representing 0 items considered to the priority queue
	+ the priority queue is sorted in order of highest bound
	+ this node has its bound calculated using the bound() function
	+ the function then, as long as the priority queue is not empty, pops the priority value and checks for conditions
		1. if there are no more items to consider, this node must be a leaf so we continue our search in the queue
		2. if the total weight of the node is greater than the max capacity, the colution can't be feasible so we continue our search in the queue
		3. If the bound is greater than the current max profit, the node is promising so we expand it
			a. make a "yes" child at the next level where we add the current item being considered. if this child is able to be a solution (even if not the correct solution), and it has a profit higher than the current maxprofit, 
			we update our max profit to this child's profit and add it to the queue
			b. make a "no" child and add it to the queue without adding the current item
		4.if the node is note promising, we continue our search in the queue
	+ at every iteration, we need to re-heapify our vector to keep the priority value on top


