Christopher Brian Tjondro

a) The program is run by simply executing the file, one could pass in the messages to encode in discrete lines in a file.
By default, the program runs using monograms.txt
i.e. 
sample_input.txt
Hello!
This is the huffman encoder/decoder sim.
------------------ sample_input.txt END -----------------

As for how my program runs/performs its task it follows these steps.
Setup process: 
	1. Get monograms.txt or its equivalent
	2. Build a map<char,double> char_freq
	3. Build a priority queue (minHeap) of Nodes (a Node contains left ptr, right ptr, a char and a double (char's frequency)).
	4. Turn the priority queue into a huffman tree
	5. Create a map<char,string> of chars with their huffman code from the huffman tree.
Encoding:
	1. Take line from stdin. 
	2.Given a map<char,string> of chars with their huffman code, convert each character in the line to its huffman code equivalent.
	3. return the resulting string.
Decoding:
	1. Line is passed by another function, or alternatively can be used in a way s.t. line can be taken from stdin.
	2. Given a huffman tree with chars, traverse the tree for each character in the encoded line.
		i. when a matching character is found, print it.
		
b).
	Limitations: 
		- Lower case characters are turned to uppercase (aBcDe -> ABCDE).
		- Non-alphabetical characters turned to whitespaces (a.B,c/D-e -> A B C D E).
		- Since the frequency for ' ' is harcoded, if another monograms.txt file were used containing ' ', there would be one unnecessary
		entry in the map and node in the priority queue.
	
	As for how whitespaces are handled internally, I created a pair<char,double> of <' ',10^7) and added it to the map char_freq, the map containing characters and their frequencies. I did
this because whitespaces are guaranteed to occur within a sentence, and such should take relatively high frequency values (and therefore a short representation like '1'). Realistically 
though, it doesn't really matter what frequency value I put in for ' ': as long as it is in char_freq, I can encode/decode it just the same eventually.

c). Test Cases Results for program

i) All lower case.
	Input line: abcdef

	input text   --->  ABCDEF  <---
	encoded text ---]  0111101011000010110101110011000101  [---
	plain text   ===>  ABCDEF  <===

ii) Mix of upper and lower case.
	Input line: aBcDeFg

	input text   --->  ABCDEFG  <---
	encoded text ---]  0111101011000010110101110011000101000100  [---
	plain text   ===>  ABCDEFG  <===


iii) Mix of upper and lower case with spaces.
Input line: Algorithms and Data Structures

	input text   --->  ALGORITHMS AND DATA STRUCTURES  <---
	encoded text ---]  011110111010001000110101000011000000000110010010100110111101010010111101011101111000001111101001000001000001010001011000000101001000001101001  [---
	plain text   ===>  ALGORITHMS AND DATA STRUCTURES  <===


iv)  Mix of upper and lower case with spaces and non-alphabetical characters.
	Input line: CPSC 2150 is taught by Dr. Gladys Monagan

	input text   --->  CPSC      IS TAUGHT BY DR  GLADYS MONAGAN  <---
	encoded text ---]  00101101110010100100101111111101100010011000001111001010000100000110000101011000101101101011101000110001000111010111101011101011010100110010010110101010011110001000111101010  [---
	plain text   ===>  CPSC      IS TAUGHT BY DR  GLADYS MONAGAN  <===
	
v) Only Symbols/non-alphabetical characters to showcase limitations. (Note that because the ' ' has a high hardcoded frequency value, this otherwise long string is shortened to the number of chars.
	Input line: %#%$^%&%^*^%%$#&%^&&*)_(($%^&*()*(*^%^&*%()<>?>?<><?<?><?><???

	input text   --->                                                                  <---
	encoded text ---]  11111111111111111111111111111111111111111111111111111111111111  [---
	plain text   ===>
	
Note: These exact tests were run with screenshots and results are in output.txt.