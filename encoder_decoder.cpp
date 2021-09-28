// Name: Christopher Brian Tjondro
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include<queue>//priority queue
using std::cout;
using std::cin;
using std::getline;
using std::endl;
using std::string;

//This is the node used in the min heap.
struct Node{
	Node* left;
	char a; //character
	double f; //frequency
	Node* right;
};

/*
Comparator function for two nodes in the min heap
*/
struct myComparator{
	bool operator()(Node* p, Node* q){
		return (p->f > q->f); 
	}
};
/*
//Takes string, removes punctuation (replaces with space), returns uppercased.
Postcondition: Returns the uppercased and non-alphabetic characters (except space) removed.
*/
string modLine(string line){
	string nLine = "";
	for(unsigned int i=0;i<line.length();i++){
		char a = std::toupper(line[i]);
		if((int(a-'A') >= 0  ) && (int(a-'A') <= int('Z' - 'A'))){//is an alphabetic char.
			nLine+=a;
		}
		else
			nLine+=" ";//turn anything else into whitespace.
		}
	return nLine;
}
/*
Prints the contents of a map<char,double> in [ key : val ] format.
Postcondition: Map's content is printed.
*/
#ifndef NDEBUG
void printMapCD(std::map<char,double> char_freq){
	   std::map<char,double> :: iterator i;
   for(i = char_freq.begin(); i!= char_freq.end();i++)//Prints the table.
	   cout<<i->first<<" : "<<i->second<<endl;
}
#endif
/*
Prints the contents of a map<char,string> in [ key : val ] format.
Postcondition: Map's content is printed.
*/
#ifndef NDEBUG
void printMapCS(std::map<char,string> c){//Prints content of a map in [ key : val ] format.
	   std::map<char,string> :: iterator i;
   for(i = c.begin(); i!= c.end();i++)//Prints the table.
	   cout<<i->first<<" : "<<i->second<<endl;
}
#endif
/*
Builds a map<char,double> from an input file.
Precondition: file exists, follows [char double] format.
Postcondition: map is returned.
*/
std::map<char,double> inputMap(string fileName){
	std::map<char,double> char_freq; // Map of characters to its frequency.
	char a;
    double f;
    std::ifstream in;
    in.open(fileName);
    char_freq.insert( std::pair<char,double> (' ',10000000.0) );//Space is some big number to ensure it has highest priority. 
	//(It is the only character guaranteed to occur in a sentence).
    while(in>>a){
		in>>f;
		char_freq.insert( std::pair<char,double> (a,f) );
	}
	return char_freq;
}
/*
Fills a priority queue (min heap) with Nodes containing char, double from the elements of a map<char,double>
Postcondition: The priority_queue is filled.
*/
void fillMinHeap(std::priority_queue<Node,std::vector<Node*>,myComparator> &pq,std::map<char,double> cf){
	std::map<char,double> :: iterator i;
    for(i = cf.begin(); i!= cf.end();i++){//pushes Nodes onto the priority_queue
		pq.push(new Node{nullptr,i->first,i->second,nullptr});
	}
}
/*
Given a priority queue (min heap) containing Nodes containing char,double, turn that priority queue into a huffman tree.
Postcondition: The priority_queue is now a huffman tree.
*/
void buildHuffmanTree(std::priority_queue<Node,std::vector<Node*>,myComparator> &pq){
	int i = -1 + 2*pq.size(); //2n-1 nodes in huffman tree for n leaves i.e. number of entries in the original frequencies file.
	if(i<2){
		return;//invalid
	}
	while(i>0){//adds appropriate parents
		Node* leftChild = pq.top();
		pq.pop();
		Node* rightChild = pq.top();
		pq.pop();
		Node* parentNode = new Node{leftChild,'-',leftChild->f + rightChild->f,rightChild};//Parent node's dummy char is -, freq is lChild->freq + rChild->freq
		pq.push(parentNode);
		i--;
	}
}
/*
Encodes a character given a huffman tree, and returns the resulting code implicitly in the string result.
Postcondition: result is returned implicitly in the string result.
*/
void encode(char a,Node* n,string tmp,string& result){//string result functions like a static variable, only initialized when a match is found.
	if(n==nullptr){//Any failed path yields nothing.
		return;
	}
	if(n->a == a){
		result+=tmp;//Replace final with the accumulated bits
		return;
	}
	encode(a,n->left,tmp+'0',result);//left traverse
	encode(a,n->right,tmp+'1',result);//right traverse
}
/*
Builds a map<char,string> of a character and its huffman code given a map<char,double> containing the same characters, and the corresponding huffman tree's root node.
Postcondition: The map of characters and their respective huffman codes is implicitly returned.
*/
void buildCodeTable(std::map<char,double> char_freq,Node* root,std::map<char,string> &codeTable){
	if(root == nullptr)
		return;
	std::map<char,double> :: iterator i;
    for(i = char_freq.begin(); i!= char_freq.end();i++){//pushes Nodes onto the priority_queue
		string result = "";
		encode(i->first,root,"",result);//encode each char, resulting code implicitly returned in the string result.
		codeTable.insert(std::pair<char,string> (i->first,result) );//Push the char and is corresponding code into the map<char,string>
	}
}
/*
Encodes a line given the map<char,string> containing each char and its corresponding huffman code.
Postcondition: The encoded line is retured.
*/
string encodeLine(std::map<char,string> ct,string line){
	string encoded = "";
	for(char a:line){
		encoded+=ct[a];//append the code to the result.
	}
	return encoded;
}
/*
Decodes a line given the root node of a huffman tree containing chars.
Postcondition: The decoded line is retured.
*/
string decodeLine(string eLine,Node* originalRoot,string fileName){
	string decoded = "";
	Node *root = originalRoot;
	if(root == nullptr)//do nothing.
		return "";
	for(char a:eLine){
		if(a == '0'){
			root = root->left;//traverse left
		}
		else{
			root = root->right;//traverse right
		}
		
		if(root == nullptr){
			return "ERROR! Character input does not exist in "+ fileName +".\n";//in case user inputs invalid combination of 1's and 0's.
		}
		else if(root->a != '-'){//if it is a valid leaf node as opposed to a dummy parent node, add the char.
			decoded+=(root->a);
			root = originalRoot;//reset root to top, on to the next word.
		}
	}
	return decoded;
}

int main(int argc, char* argv[]) {
	const std::string DEFAULT_FILE_NAME = "monograms.txt";
	string line;
	std::string fileName = DEFAULT_FILE_NAME;
	if (argc > 1) {//Alternative Filename
		fileName = argv[1];
	}
	std::map<char,double> char_freq = inputMap(fileName);//This contains all chars and their corresponding frequencies.
	
	// if something went wrong and the map does not have elements, just quit
	// could have thrown an exception and it's caught and dealt with
	if (char_freq.empty()){
		std::cerr << "ERROR: problems reading " << fileName << " ... bye\n";
		return 1;
	}
	#ifndef NDEBUG	
	cout<<"Character and frequency map obtained from "<<fileName<<" :"<<endl;
	cout<<"Character : Frequency"<<endl;

		printMapCD(char_freq);
	#endif
	
	std::priority_queue<Node,std::vector<Node*>,myComparator> minHeap; //This is a minHeap, later it gets turned into a huffman tree.
	
	fillMinHeap(minHeap,char_freq);//Fills the minHeap with nodes of chars and frequencies
	
	buildHuffmanTree(minHeap);//Modifies the minHeap into a huffman tree.
	
	std::map<char,string> codeTable;//Contains the chars and their corresponding huffman codes.
	
	buildCodeTable(char_freq,minHeap.top(),codeTable);//Builds a table of chars and their huffman codes.
	
	#ifndef NDEBUG
	cout<<"Character and Huffman Code map derived from "<<fileName<<" :"<<endl;
	cout<<"Character : Code"<<endl;

		printMapCS(codeTable);
	#endif
	
	cout<<"Input line: ";
	while (getline(cin, line)) {
		line = modLine(line);
		cout << "\ninput text   --->  " << line << "  <---\n";
		string encodedText = encodeLine(codeTable,line);
		cout << "encoded text ---]  " << encodedText << "  [---\n";
		string plainText = decodeLine(encodedText,minHeap.top(),fileName);
		cout << "plain text   ===>  " << plainText << "  <===\n";
		cout<<endl;
		cout<<"Input line: ";
	}
	return 0;
}

