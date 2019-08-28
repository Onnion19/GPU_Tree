#include "Dictionary.h"



Dictionary::Dictionary()
{
	InitItems();
}


bool Dictionary::AddRule(char item, string replace)
{
	bool correct = false; 
	if (!ItemExist(item))
		return correct; 

	rule newRule; 
		newRule._rule = replace;
		newRule._branchesInsindeRule = FindNewBranchesOnRule(replace);



	map<char,rule>::iterator iter = _rules.find(item);
	if (iter == _rules.end())
	{
		//String rule
		pair<char, rule> newItem;
		newItem.first = item; newItem.second = newRule;

		_rules.insert(newItem);
		cerr << "Rule added: " << item << " --> " << replace <<" --> " << newRule._branchesInsindeRule<< " Branches inside the rule"<<  endl;

		//Numeric rule
		numericRule nR; nR._rule = TranslateStringToShort(replace); nR._branchesInsindeRule = newRule._branchesInsindeRule;
		pair<unsigned short, numericRule> newNumericItem; newNumericItem.first = TranslateCharToShort(item); newNumericItem.second = nR;

		_numericRules.insert(newNumericItem);
		cerr << "Numeric Rule added: " << endl; 
		PrintNumericRule(newNumericItem.first,nR);

		correct = true;
	}
	else
		cerr << "A rule already exist for this item :: Item: " << item << endl;

	
	return correct;
}


string Dictionary::Replace(char item)const
{
	string output; 
	output.push_back(item);
	map<char, rule>::const_iterator iter = _rules.find(item);

	if (iter != _rules.end())
		output = iter->second._rule;  

	return output;
}

vector<unsigned short> Dictionary::Replace(unsigned short item)const
{
	vector<unsigned short> output; 
	output.push_back(item); 
	map<unsigned short, numericRule>::const_iterator iter = _numericRules.find(item);
	if (iter != _numericRules.end())
	{
		output.clear(); 
		output = iter->second._rule;
	}

	return output;
	
}

vector<unsigned short> Dictionary::TranslateStringToShort(string input, bool debug )const
{
	vector<unsigned short> translation;
	vector<string> errors;
	for (int i = 0; i < input.size(); i++)
	{
		short position = ItemPosition(input[i]);
		if(position != -1)
			translation.push_back(position);

		//Only Debug porpuses code:
		if (debug)
		{
			if (position != -1)	cerr << input[i]<<" --> "<<position << " ";
			else errors.push_back("The following char has not founded: " + input[i]);
		}
		//End of debug
	}

	if (debug) cerr << endl;
	if (debug && errors.size())
	{
		cerr << "---------------------------------------------" << endl;
		cerr << "List of errors: " << endl;
		for (int i = 0; i < errors.size(); i++)
			cerr << "\t"<<errors[i] << endl;
		cerr << "---------------------------------------------" << endl;
	}


	return translation;
}


unsigned short Dictionary::TranslateCharToShort(char input, bool debug)const
{
	short retorn = ItemPosition(input);
	if (debug)
	{
		if (retorn != -1)	cerr << input << " --> " << retorn << " ";
		else cerr << "The following char has not founded: " << input << endl;
	}
	
	if (retorn == -1) retorn=0;//0 utilitzat per demarcar error.
	return retorn;
}


void Dictionary::ClearRules()
{
	cerr << "Clear Rules" << endl;
	_rules.clear();
}


void Dictionary::ClearAll()
{
	ClearRules();
	cerr << "Clear Items" << endl;
	_items.clear();
}



void Dictionary::PrintItems()const
{
	cout << "Items" << endl;
	for (int i = 0; i < _items.size();i++) {
		cout << _items[i] << endl;
	}
}

void Dictionary::PrintRules()const
{
	cout << "Rules" << endl; 
	map<char, rule>::const_iterator iter = _rules.begin();
	while (iter != _rules.end())
	{
		cout << "Rules for: " << iter->first << endl; 
		cout << "\t" << iter->second._rule << "\t Has " << iter->second._branchesInsindeRule << " Branches inside" << endl;
		cout << endl;
		iter++;
	}
}

void Dictionary::PrintAll()const
{
	cout << "===== ITEMS =====" << endl;
	PrintItems();
	cout << "===== RULES =====" << endl;
	PrintRules();
}
//PRIVATE

void Dictionary::InitItems()
{
	ifstream file; 
	file.open(symbol_path); 
	if (file.fail()|| !file.is_open())
	{
		cerr << "Symbols.txt can't be open" << endl;
		throw ("SymbolsFileRead");
	}
		
	char item; 
	int number; 
	file >> number;
	while (!file.eof())
	{
		cerr << "Symbols readed: " << number << endl;
		file >> item;		
		cerr << "Read symbol: " << item << endl; 

		_items.push_back(item);

		if(!file.eof())
			file >> number;
	}

	cout << "Number of items readed: " << _items.size() << endl;
	file.close();
}


unsigned short Dictionary::FindNewBranchesOnRule(string newRule)
{
	unsigned short newBranches = 0; 
	for (int i = 0; i < newRule.length(); i++)
	{
		if (newRule[i] == '[')newBranches++;
	}

	return newBranches;
}


bool Dictionary::ItemExist(char node)const
{
	int i = 0; 
	while (i < _items.size() && _items[i] != node)
		i++;
	
	
	return i < _items.size();
}

int Dictionary::ItemPosition(char item)const
{
	int i = 0;
	while (i < _items.size() && _items[i] != item)
		i++;


	i < _items.size() ? i = i+1 : i = -1;//i+1 perquè el primer item volem que sigui el 1 i no el 0 (reservem el 0 per gestio errors )
	return i; 
}


void Dictionary::PrintNumericRule(unsigned int item, numericRule _numRule)const
{
	cerr << "\t" << item << " --> "; 
	for (int i = 0; i < _numRule._rule.size(); i++)
		cerr << _numRule._rule[i] << " "; 
	cerr<< " --> " << _numRule._branchesInsindeRule << " Branches inside the rule" << endl;
}