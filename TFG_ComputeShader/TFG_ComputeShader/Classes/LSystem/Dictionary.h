#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <fstream>
using namespace std;

//! Dictionary class to store rules and Symbols for the L-System.
/*!
Dictionary is used to store which Symbols understands the L-System and which rules are applied. The best way to populate the class 
is providing a file with all the data. 
For the symbols, file must be the following format: 
Number of symbol Symbol
	Example: 
		0 X
		1 y

For the rules, the format must be: 
Axiom : Rule
	Example: 
		X : yX
		y : XX 
*/
class Dictionary
{
public:
	Dictionary();

	//! Adds a new rule to the dictionary and precoumptes the number of branches created inside the rule. If the string to search does not exist, then the rule is ingored
	/*!
	\param item <char> string to be replaced
	\param replace <string> string to be placed
	\return bool If the rules is added or not
	*/
	bool AddRule(char item, string replace); 

	//! Search for a valid replacement
	/*!
		\param item <char> input to be replaced if a rule exist
		\return <string> the new string if it was replaced, Otherwise the old string;
	*/
	string Replace(char item) const; 

	//! Numeric Version for Replace Search for a valid replacement
	/*!
	\param item <char> input to be replaced if a rule exist
	\return <string> the new string if it was replaced, Otherwise the old string; 
	*/
	vector<unsigned short> Replace(unsigned short item)const;

	//! Transforms the string of char, to a sequence of numbers
	/*!
	 Transformation rule: Position of the char stored in vector of items = number to be translated. If the item is not found, it gets ignored, and an error message is prompt as long as the debug option is enabled;
	 \param input <string> String to be translated to numbers
	 \param debug <bool> Wehter it is enabled it promps useful information about the translate process. 
	 \return vector <unsigned int> Translated string into int.
	*/
	vector<unsigned short> TranslateStringToShort(string input, bool debug = false)const;

	//! Transform a given char to number.
	/*!
	Transformation rule: Position of the char stored in vector of items = number to be translated. If the item is not found, it gets ignored, and an error message is prompt as long as the debug option is enabled;
	\param input <char> char to be translated to number
	\param debug <bool> Wehter it is enabled it promps useful information about the translate process.
	\return unsigned int Translated string into int. Return 0 if no conversion founded
	*/
	unsigned short TranslateCharToShort(char input, bool debug = false)const;
	//! Removes all the rules
	void ClearRules();

	//!Removes all the rules and items 
	/*!
		If you only want to clear the rules, consider using ClearRules
	*/
	void ClearAll();
	
	//!Print all the symbols and rules which are in the Dictionary
	void PrintItems()const; 
	//!Print all the rules which are in the Dictionary
	void PrintRules()const;
	//!Print all the Symbols and all the Rules
	void PrintAll()const;

private: 
	
	struct rule {
		string _rule; 
		unsigned short _branchesInsindeRule; 	
	};
	struct numericRule {
		vector<unsigned short> _rule;
		unsigned short _branchesInsindeRule;
	};

	vector<char> _items; 
	vector<int> _numericItems;
	map<char, rule> _rules; 
	map<unsigned short, numericRule> _numericRules;
	const string _path = "Diccionari.txt";
	const string symbol_path = "Symbols.txt";

	vector<int>_num_items; 
	map<int, vector <int> > _num_rules;

	void InitItems();
	unsigned short FindNewBranchesOnRule(string rule);
	bool ItemExist(char node)const;
	int ItemPosition(char item)const;

	void PrintNumericRule(unsigned int item, numericRule _numRule)const;


};
