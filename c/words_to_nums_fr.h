#include <vector>
#include <cstdlib>
#include <iostream>
#include <string>
#include <unordered_map>
#include <locale> //std::locale, std::tolower
#include <algorithm>

#include "bigint.h"

class words_to_nums_fr{
public:
  words_to_nums_fr(){
    initialize_numvalse();
    
    for (int k = 0; k < numbers.size(); k++){
      number_units[numbers[k] ] = numbervals[k];
      if(k>0)//plurals
                number_units[numbers[k]+"s"] = numbervals[k];
    }
    bigint value = ZERO;
    for(int k = 0; k < small_numbers.size();k++){
      value = value.add(ONE);
            number_units[small_numbers[k] ] = value;
    }
    
    //Collections.addAll(nonbiglist,small_numbers);
    nonbiglist.insert(nonbiglist.end(),
		      small_numbers.begin(),
		      small_numbers.end());
    }


  bigint converter(std::string value){
    //string parsing
    std::string valuetosplit = value;
    std::transform(valuetosplit.begin(),valuetosplit.end(),valuetosplit.begin(), ::tolower);
    valuetosplit = ReplaceAll(valuetosplit," et ","-et-");
    valuetosplit = ReplaceAll(valuetosplit,","," "); // for text with commas.

    std::vector<std::string> splitvals = SPLIT(valuetosplit," ");
    
    //------------------ error checks ------------------------------
    //std::string ones[] = Arrays.copyOfRange(small_numbers,0,9);
    //std::string tenplusteens[]=Arrays.copyOfRange(small_numbers,9,small_numbers.size());
    
    std::string errorS = "Misspelling at word number ";
    std::string errorO = "Improper order at word number ";
    int k;
    
    bool ordering=true;
    
    for(k = 0 ; k < splitvals.size();k++)
      {
	//TODO: combine some of these if statments and reduce the repeated boolean calculations
	if(number_units.find(splitvals[k])== number_units.end()){
	      errorS += std::to_string(k+1)+": "+splitvals[k];
	      //System.out.println(errorS);
	      std::cout << errorS << std::endl;
	      return ZERO;
            }
	    
      }
    
    // check ordering (not complete):
    if(splitvals.size() > 1){
            for(k = 0 ; k < splitvals.size()-1;k++){
	      if(number_units[splitvals[k] ].tostring().size() ==
		 number_units[splitvals[k+1] ].tostring().size() ){
		ordering = false;
		break;
                }
	      
	      if(number_units[splitvals[k] ].tostring().size() == 1 &&
		 (number_units[splitvals[k+1] ].tostring().size() == 2)){
                    ordering = false;
                    break;
	      }
	      
	      if(number_units[splitvals[k] ].tostring().size() == 2 &&
		 (number_units[splitvals[k+1] ].tostring().size() == 3)){
                    ordering = false;
                    break;
	      }
	      
            }
        }
    k++;
    //System.out.println(k);
    if(!ordering){
      errorO += std::to_string(k)+": "+splitvals[k-1]+" "+splitvals[k];
      std::cout << errorO << std::endl;
      //      System.out.println(errorO);
      return ZERO;
    }
    
    
    return conv_num(splitvals);
  }
  

  
  std::string reverse(bigint value){
    std::string numstr = value.tostring();
    int len = numstr.size();
    
        bigint HUNDRED = TEN;
        HUNDRED = HUNDRED.multiply(TEN);
	
        if(value < HUNDRED){
	  return reverse_conv_hundreds(numstr);
        }
	
        int partitions = len / 3;
        int leftovers = len % 3;
        //System.out.println(leftovers);
        if(partitions <= 1 && leftovers == 0)
            return reverse_conv_hundreds(numstr);

        std::string result = "";
        if(leftovers > 0)
            result += reverse_conv_hundreds(numstr.substr(0,leftovers));
        if(leftovers != 0)
            result += " "+numbers[--partitions];
        else
            --partitions;
        int index = leftovers;
        while(partitions > 0){
            result += " ";
            result += reverse_conv_hundreds(numstr.substr(index,3));
            if(reverse_conv_hundreds(numstr.substr(index,3)) != "")
                result += " "+numbers[--partitions];
            else
                --partitions;
            index += 3;
        }

        result += " ";
        result += reverse_conv_hundreds(numstr.substr(index,numstr.size()));
        //fix double spaces here because of laziness.
        //std::string after = before.trim().replaceAll(" +", " ");

	std::string::iterator new_end = std::unique(result.begin(),result.end(),
						    [](char lhs,char rhs) ->
						    bool {return (lhs==rhs) && (lhs == ' ');} );
	result.erase(new_end,result.end());
	result = TRIM(result);
	
	//result = result.trim().replaceAll(" +"," ");
	return result;
  }


private:
  bigint ONE = bigint().ONE();
  bigint TEN = bigint().TEN();
  bigint ZERO = bigint().ZERO();
  
  std::string ReplaceAll(std::string str, const std::string& from,const std::string& to){
    size_t start_pos = 0;
    while((start_pos = str.find(from,start_pos)) != std::string::npos){
      str.replace(start_pos,from.size(),to);
      start_pos += to.size();
    }
    return str;
  }


  std::vector<std::string> SPLIT(const std::string& s, std::string delim) {
    auto i = 0;
    auto pos = s.find(delim);
    std::vector<std::string> v;
    while (pos != string::npos) {
      v.push_back(s.substr(i, pos-i));
      i = ++pos;
      pos = s.find(delim, pos);

      if (pos == string::npos)
	v.push_back(s.substr(i, s.size()));
    }
    return v;
  }

  std::string TRIM(std::string s){
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
	  return !std::isspace(ch);
	}));

    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
	  return !std::isspace(ch);
	}).base(), s.end());

    return s;
  }

  
  //    public int
  std::vector<std::string> small_numbers = {"un",
					    "deux",
					    "trois",
					    "quatre",
					    "cinq",
					    "six",
					    "sept",
					    "huit",
					    "neuf",
					    "dix",
					    "onze",
					    "douze",
					    "treize",
					    "quatorze",
					    "quinze",
					    "seize",
					    "dix-sept",
					    "dix-huit",
					    "dix-neuf",
					    "vingt",
					    "vingt-et-un",
					    "vingt-deux",
					    "vingt-trois",
					    "vingt-quatre",
					    "vingt-cinq",
					    "vingt-six",
					    "vingt-sept",
					    "vingt-huit",
					    "vingt-neuf",
					    "trente",
					    "trente-et-un",
					    "trente-deux",
					    "trente-trois",
					    "trente-quatre",
					    "trente-cinq",
					    "trente-six",
					    "trente-sept",
					    "trente-huit",
					    "trente-neuf",
					    "quarante",
					    "quarante-et-un",
					    "quarante-deux",
					    "quarante-trois",
					    "quarante-quatre",
					    "quarante-cinq",
					    "quarante-six",
					    "quarante-sept",
					    "quarante-huit",
					    "quarante-neuf",
					    "cinquante",
					    "cinquante-et-un",
					    "cinquante-deux",
					    "cinquante-trois",
					    "cinquante-quatre",
					    "cinquante-cinq",
					    "cinquante-six",
					    "cinquante-sept",
					    "cinquante-huit",
					    "cinquante-neuf",
					    "soixante",
					    "soixante-et-un",
					    "soixante-deux",
					    "soixante-trois",
					    "soixante-quatre",
					    "soixante-cinq",
					    "soixante-six",
					    "soixante-sept",
					    "soixante-huit",
					    "soixante-neuf",
					    "soixante-dix",
					    "soixante-et-onze",
					    "soixante-douze",
					    "soixante-treize",
					    "soixante-quatorze",
					    "soixante-quinze",
					    "soixante-seize",
					    "soixante-dix-sept",
					    "soixante-dix-huit",
					    "soixante-dix-neuf",
					    "quatre-vingts",
					    "quatre-vingt-un",
					    "quatre-vingt-deux",
					    "quatre-vingt-trois",
					    "quatre-vingt-quatre",
					    "quatre-vingt-cinq",
					    "quatre-vingt-six",
					    "quatre-vingt-sept",
					    "quatre-vingt-huit",
					    "quatre-vingt-neuf",
					    "quatre-vingt-dix",
					    "quatre-vingt-onze",
					    "quatre-vingt-douze",
					    "quatre-vingt-treize",
					    "quatre-vingt-quatorze",
					    "quatre-vingt-quinze",
					    "quatre-vingt-seize",
					    "quatre-vingt-dix-sept",
					    "quatre-vingt-dix-huit",
					    "quatre-vingt-dix-neuf",
					    "cent"};

  std::vector<std::string> numbers = {"mille",
				      "million",
				      "milliard",
				      "billion",
				      "billiard",
				      "trillion",
				      "trilliard",
				      "quadrillion",
				      "quadrilliard",
				      "quintillion",
				      "quintilliard",
				      "sextillion",
				      "sextilliard",
				      "septillion",
				      "septilliard",
				      "octillion",
				      "octilliard",
				      "nonillion",
				      "nonilliard",
				      "decillion",
				      "decilliard",
				      "undecillion",
				      "undecilliard",
				      "duodecillion",
				      "duodecilliard",
				      "tredecillion",
				      "tredecilliard",
				      "quattuordecillion",
				      "quattuordecilliard",
				      "quindecillion",
				      "quindecilliard",
				      "sexdecillion",
				      "sexdecilliard",
				      "septendecillion"};


  std::vector<bigint> numbervals;

  bigint THOUSAND = bigint("1000");
  void initialize_numvalse() {
    numbervals.resize(numbers.size());
    numbervals[0] = THOUSAND;
    for (int k = 1; k < numbers.size(); k++) {
      numbervals[k] = numbervals[k - 1].multiply(THOUSAND);
    }
  }

  unordered_map<std::string, bigint> number_units;

    // THE CONVERTER
    //converter for numbers between 1 and 999
    //input and string array of length between 1 and 4;
    // TODO: make this recursive and throw exceptions when "hundred" is not in the right place.
  bigint convert_hundreds(std::vector<std::string> value){
    if(value.size()==1)
      return number_units[value[0] ];
    else if(value.size()==2 && (value[0] == "cent")){
      return (number_units[value[0]].add(number_units[value[1] ]) );
    }
        else if(value.size()==2 && value[1] == "cent"){
	  return (number_units[value[0]].multiply(number_units[value[1] ]) );
        }
        else if(value.size()==3 && value[1] == "cent")
	  return ( (number_units[value[0] ].multiply(number_units[value[1] ] ))).add(number_units[value[2] ]);
        else{
	  std::cout << "improper number starting at "+value[0]+
	    " ending at "+value[value.size()-1] << std::endl;
            return ZERO;
        }
  }
  
  std::vector<std::string> nonbiglist;
  // finds the first non small unit in string array, returns -1 other
  int find_first_big(std::vector<std::string> values){
        int counter = -1;
        for(int k = 0; k < values.size();k++){
	  if(std::find(nonbiglist.begin(),nonbiglist.end(),values[k]) == nonbiglist.end()){
                counter = k;
                break;
            }
        }
        return counter;
    }

  bigint conv_small_text(std::vector<std::string> values){
        int s = find_first_big(values);
        if(s<0){
            return convert_hundreds(values);
        }
        else{
	  //std::string arrayl[] = Arrays.copyOfRange(values,0,s);
	  //std::string arrayr[] = Arrays.copyOfRange(values,s+1,values.size());
	  std::vector<std::string> arrayl(values.begin(),values.begin()+s);
	  std::vector<std::string> arrayr(values.begin()+s+1,values.end());


	  bigint result = number_units[values[s] ].multiply(convert_hundreds(arrayl));
	  result = (s < values.size()-1)? result.add(conv_small_text(arrayr)) : result.add(ZERO);
	  return result;
        }
    }

  bigint conv_num(std::vector<std::string> values){
        //base cases
        if(values.size() <= 4){
            return conv_small_text(values);
        }

        int s = find_first_big(values);

        //std::string arrayl[] = Arrays.copyOfRange(values,0,s);
        //std::string arrayr[] = Arrays.copyOfRange(values,s+1,values.size());
	std::vector<std::string> arrayl(values.begin(), values.begin()+s);
	std::vector<std::string> arrayr(values.begin()+s+1, values.end());

        return (number_units[values[s] ].multiply(convert_hundreds(arrayl))).add(conv_num(arrayr));
    }

    //wrapper for recursive function
    //returns zero if there is a mispelling

     std::string reverse_conv_hundreds(std::string value){
        if(value == "000"){
            return "";
        }
        int intval = std::stoi(value);
        //System.out.println(value);
        if(intval <= 100)
            return small_numbers[intval-1];
        int hunds = intval / 100;
        int tens = intval % 100;

        //System.out.println(tens-1);
        std::string result = "";
        result += small_numbers[hunds-1];
        result += " cent ";
        if(tens > 0)
            result += small_numbers[tens-1];
        return result;
        //return small_numbers[hunds - 1] +" cent "+ small_numbers[tens-1];
    }

};
