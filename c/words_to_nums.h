#include <vector>
#include <cstdlib>
#include <iostream>
#include <string>
#include <unordered_map>
#include <locale> //std::locale, std::tolower
#include <algorithm>

#include "bigint.h"


//TODO: better memory management, use vectors instead of basic chars
//TODO: pass as reference where appropriate.
class words_to_nums {

public:
  words_to_nums() {
    initialize_numvalse();
    for (int k = 0; k < numbers.size(); k++)
      number_units[numbers[k] ] = numbervals[k] ;
    
    bigint value = ZERO;
    for (int k = 0; k < small_numbers.size(); k++) {
      value = value.add(ONE);
      number_units[small_numbers[k] ] = value;
    }
    value = value.add(ONE);
    for (int k = 0; k < tens.size(); k++) {
      number_units[tens[k] ] = value;
      value = value.add(TEN);
    }
    number_units["hundred"]= value;


    //Collections.addAll(nonbiglist, small_numbers);
    //Collections.addAll(nonbiglist, tens);
    //Collections.addAll(nonbiglist, "hundred");
    nonbiglist.insert(nonbiglist.end(),small_numbers.begin(),small_numbers.end());
    nonbiglist.insert(nonbiglist.end(),tens.begin(),tens.end());
    nonbiglist.push_back("hundred");
  }

  //wrapper for recursive function
  //returns zero if there is a mispelling
  bigint converter(std::string value) {
    //string parsing
    std::string valuetosplit = ReplaceAll(value,"-", " ");
    std::transform(valuetosplit.begin(),valuetosplit.end(),valuetosplit.begin(), ::tolower);
    valuetosplit = ReplaceAll(valuetosplit," and ", " ");
    valuetosplit = ReplaceAll(valuetosplit,",", " "); // for text with commas.
    //valuetosplit = valuetosplit.trim().replaceAll(" +"," "); //getting rid of double spaces
    std::string::iterator new_end = std::unique(valuetosplit.begin(),valuetosplit.end(),
						[](char lhs,char rhs) ->
						bool {return (lhs==rhs) && (lhs == ' ');} );
    valuetosplit.erase(new_end,valuetosplit.end());

    valuetosplit = TRIM(valuetosplit);
    std::vector<std::string> splitvals = SPLIT( valuetosplit, " ");
    
    //std::string ones[] = Arrays.copyOfRange(small_numbers, 0, 9);
    //std::string tenplusteens[] = Arrays.copyOfRange(small_numbers, 9, small_numbers.size());
    std::vector<std::string> ones(small_numbers.begin(), small_numbers.begin()+9);
    std::vector<std::string> tenplusteens(small_numbers.begin()+9,small_numbers.end());

    
    std::string errorS = "Misspelling at word number ";
    std::string errorO = "Improper order at word number ";
    int k;
    for (k = 0; k < splitvals.size() ; k++) {
      //TODO: combine some of these if statments and reduce the repeated boolean calculations
      if (number_units.find(splitvals[k]) == number_units.end()) {
	errorS += std::to_string(k + 1) + ": " + splitvals[k];
	std::cout << errorS << std::endl;
	return ZERO;
      }
      if (k < splitvals.size() - 1 &&
	  (std::find(tens.begin(),tens.end(),splitvals[k]) != tens.end()) &&
	  (splitvals[k + 1] == "hundred"))
	break;
      
      if (k < splitvals.size()  - 1 &&
	  (std::find(small_numbers.begin(),small_numbers.end(),splitvals[k]) !=small_numbers.end()) &&
	  (std::find(tens.begin(),tens.end(),splitvals[k+1]) != tens.end()) )
	  break;
      
      if (k < splitvals.size()  - 1 &&
	  (std::find(tens.begin(),tens.end(),splitvals[k]) != tens.end()) &&
	  (std::find(tenplusteens.begin(),tenplusteens.end(),splitvals[k+1]) != tenplusteens.end()) )
	break;
      
	if (k < splitvals.size()  - 1 &&
	    (std::find(ones.begin(),ones.end(),splitvals[k]) != ones.end() ) &&
	    (std::find(tenplusteens.begin(),tenplusteens.end(),splitvals[k+1]) != tenplusteens.end() ) )
	  break;
	
	if (k < splitvals.size()  - 1 &&
	    (std::find(tenplusteens.begin(),tenplusteens.end(),splitvals[k]) != tenplusteens.end() ) &&
	    (std::find(ones.begin(),ones.end(),splitvals[k+1]) != ones.end() ) )
	  break;
	
	if (k < splitvals.size()  - 1 &&
	    (std::find(tenplusteens.begin(),tenplusteens.end(),splitvals[k]) != tenplusteens.end() ) &&
	    splitvals[k + 1] == "hundred") 
	  break;
    }
    
      if (k < splitvals.size() ) {
	errorO += std::to_string(k + 1) + ": " + splitvals[k] + " " + splitvals[k + 1];
	std::cout << errorO << std::endl;
	return ZERO;
      }

      //     std::cout<< "here" <<std::endl;      
      return conv_num(splitvals);
  }

  // TODO: to string function may not work
  std::string reverse(bigint value) {
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
    if (partitions <= 1 && leftovers == 0)
      return reverse_conv_hundreds(numstr);

    
    std::string result = "";
    if (leftovers > 0)
      result += reverse_conv_hundreds(numstr.substr(0, leftovers));
    if (leftovers != 0)
      result += " " + numbers[--partitions];
    else
	  --partitions;
    int index = leftovers;
    while (partitions > 0) {
      result += " ";
      result += reverse_conv_hundreds(numstr.substr(index, 3));
      //      std::cout << "here " << numstr << std::endl;		  
      if (reverse_conv_hundreds(numstr.substr(index, 3)) != "")
	    result += " " + numbers[--partitions];
      else
	--partitions;
	  index += 3;
    }
    //    std::cout << "here 2" << std::endl;	

    result += " ";
    result += reverse_conv_hundreds(numstr.substr(index, numstr.length()));
    //fix double spaces here because of laziness.
    //std::string after = before.trim().replaceAll(" +", " ");
    //result = result.trim().replaceAll(" +", " ");
    std::string::iterator new_end = std::unique(result.begin(),result.end(),
						[](char lhs,char rhs) ->
						bool {return (lhs==rhs) && (lhs == ' ');} );
    result.erase(new_end,result.end());
    result = TRIM(result);
    /*
      
      bool BothAreSpaces(char lhs, char rhs) { return (lhs == rhs) && (lhs == ' '); }
	  
      std::string::iterator new_end = std::unique(str.begin(), str.end(), BothAreSpaces);
      str.erase(new_end, str.end());
    */
    
    //handle the and's here
    result = addys(result);
    return result;
  }
  
  
  
private:
  bigint ONE = bigint().ONE();
  bigint TEN = bigint().TEN();
  bigint ZERO = bigint().ZERO();

  std::string ReplaceAll(std::string str, const std::string& from,const std::string& to){
    size_t start_pos = 0;
    while((start_pos = str.find(from,start_pos)) != std::string::npos){
      str.replace(start_pos,from.length(),to);
      start_pos += to.length();
    }
    return str;
  }


  std::vector<std::string> SPLIT(const std::string& s, std::string delim) {
    auto i = 0;
    auto pos = s.find(delim);
    std::vector<std::string> v;
    while (pos != std::string::npos) {
      v.push_back(s.substr(i, pos-i));
      i = ++pos;
      pos = s.find(delim, pos);

      if (pos == std::string::npos)
	v.push_back(s.substr(i, s.length()));
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
  
    //    public Integer
  std::vector<std::string> small_numbers = {"one", "two", "three", "four",
					      "five", "six", "seven", "eight", "nine",
					      "ten", "eleven", "twelve", "thirteen",
					      "fourteen", "fifteen", "sixteen",
					      "seventeen", "eighteen", "nineteen"};
  
  
  std::vector<std::string> tens = {"twenty", "thirty", "forty", "fifty",
				     "sixty", "seventy", "eighty", "ninety"};
  
  std::vector<std::string> numbers = {"thousand", "million", "billion", "trillion", "quadrillion",
					"quintillion", "sextillion", "septillion", "octillion",
					"nonillion", "decillion", "undecillion", "duodecillion",
					"tredecillion", "quattuordecillion", "quindecillion",
					"sexdecillion", "septendecillion", "octodecillion",
					"novemdecillion", "vigintillion", "unvigintillion",
					"duovigintillion", "trevigintillion", "quatvigintillion",
					"quinvigintillion", "sexvigintillion", "septenvigintillion",
					"octovigintillion", "novemvigintillion", "trigintillion",
					"untrigintillion","duotrigintillion"};
  

  std::vector<bigint> numbervals; 
  bigint THOUSAND = bigint("1000");

  void initialize_numvalse() {
    numbervals.resize(numbers.size());
    numbervals[0] = THOUSAND;
    for (int k = 1; k < numbers.size(); k++) {
      numbervals[k] = (numbervals[k - 1]).multiply(THOUSAND);
    }
  }
  
  std::unordered_map<std::string, bigint> number_units;
  
    // THE CONVERTER
    //converter for numbers between 1 and 999
    //input and string array of length between 1 and 4;
    // TODO: make this recursive and throw exceptions when "hundred" is not in the right place.
  bigint convert_hundreds(std::vector<std::string> value) {
        if (value.size() == 1)
            return number_units[value[0]];
        else if (value.size() == 2) {
            if (value[1] == "hundred")
                return (number_units[value[0]].multiply(number_units[value[1] ]));
            else
                return (number_units[value[0] ].add(number_units[value[1] ]));
        } else if (value.size() == 3)
	  return ((number_units[value[0] ].multiply(number_units[value[1] ]))).add(number_units[value[2] ]);
        else if (value.size() == 4)
            return ((number_units[value[0] ].multiply(number_units[value[1] ]))).add(number_units[value[2] ]).add(number_units[value[3] ]);
        else {
	  std::cout << "improper number starting at "+value[0]+
	    " ending at "+value[value.size()-1] << std::endl;
            return ZERO;
        }
    }

  std::vector<std::string> nonbiglist;

    // finds the first non small unit in string array, returns -1 other
  int find_first_big(std::vector<std::string> values) {
        int counter = -1;

        for (int k = 0; k < values.size(); k++) {
	  if (std::find(nonbiglist.begin(), nonbiglist.end(), values[k]) == nonbiglist.end()) {
	    //std::cout << "here " << values[k] <<  std::endl;	    
	    counter = k;
	    break;
	  }
        }
        return counter;
  }

  bigint conv_small_text(std::vector<std::string> values) {
    int s = find_first_big(values);
    //std::cout << "here bitch " << s << std::endl;

    if (s < 0) {
      return convert_hundreds(values);
    } else {
      
      //std::string arrayl[] = Arrays.copyOfRange(values, 0, s);
      //    std::string arrayr[] = Arrays.copyOfRange(values, s + 1, values.size());
      std::vector<std::string> arrayl(values.begin(),values.begin()+s);
      std::vector<std::string> arrayr(values.begin()+s+1, values.end());
      bigint result = number_units[values[s] ].multiply(convert_hundreds(arrayl));
      result = (s < values.size() - 1) ? result.add(conv_small_text(arrayr)) : result.add(ZERO);
      return result;
    }
  }
  
  bigint conv_num(std::vector<std::string> values) {
    //base cases
    if (values.size() <= 4) {
      return conv_small_text(values);
    }

    
    int s = find_first_big(values);
    if(s == values.size() - 1){
      std::vector<std::string> arrayl(values.begin(),values.begin() + s);
      return conv_small_text(arrayl).
	multiply(number_units[values[s] ]);
    }    

	
    //	std::vector<std::string> arrayl = Arrays.copyOfRange(values, 0, s);
    //std::vector<std::string> arrayr = Arrays.copyOfRange(values, s + 1, values.size());
    std::vector<std::string> arrayl(values.begin(),values.begin() + s);
    std::vector<std::string> arrayr(values.begin()+s+1,values.end());
	
        return (number_units[values[s] ].multiply(convert_hundreds(arrayl))).add(conv_num(arrayr));
    }
  
  
  std::string reverse_conv_hundreds(std::string value) {
    if (value == "000") {
      return "";
    }
    int intval = std::stoi(value);
        if (intval < 20)
	  return small_numbers[intval - 1];
        int hunds = intval / 100;
	int tenval = (intval % 100);
        int ones = (intval % 100) % 10;
	
        std::string result = "";
        if (hunds > 0)
	  result += small_numbers[hunds - 1] + " hundred ";
        if (tenval != 0 && tenval < 20) {
	  result += small_numbers[tenval - 1];
        }
        if (tenval >= 20 && tenval <= 99) {
	  result += tens[(tenval / 10) - 2];
	  if (tenval % 10 > 0) {
	    result += " ";
	    result += small_numbers[(tenval % 10) - 1];
	  }
        }
	
        return result;

    }
  
  
  std::string addys(std::string result){
    result = ReplaceAll(result,"hundred ", "hundred and ");
    for(int k = 0; k < numbers.size();k++){
      result = ReplaceAll(result,"and "+numbers[k],numbers[k]);
    }
    
    for(int k = 0; k < numbers.size();k++){
      std::string from = " "+numbers[k];
      std::string to = " "+numbers[k];
      to += ",";
      result = ReplaceAll(result,from,to);
    }
    
    if(result.substr(result.length() - 1) == ",")
      result = result.substr(0,result.length()-1);
    return result;
    
  }

};
