#include <vector>
#include <cstdlib>
#include <iostream>
#include <string>
#include <unordered_map>
#include <locale> //std::locale, std::tolower
#include <algorithm>

#include "bigint.h"

class words_to_nums_es{
public:
  words_to_nums_es(){
    initialize_numvalse();
    
    for (int k = 0; k < numbers.size(); k++){
      number_units[numbers[k] ] = numbervals[k];
      number_units[numbers[k]+"es" ] = numbervals[k];
      number_units[shortscale_names[k] ] =  numbervals[k];//unaccented
      number_units[shortscale_names[k]+"es" ] = numbervals[k];	//unaccented plurals
      //System.out.println(numbers[k] + "\t" + numbervals[k].tostd::string());
      //	    if(k % 2 == 1){
      //	number_units.put(numbers[k]+"es",numbervals[k]);//plurals
      //	number_units.put(shortscale_names[k / 2], numbervals[k]);//unaccented
            //	number_units.put(shortscale_names[k / 2]+"es", numbervals[k]);	//unaccented plurals
            //}
    }
    bigint value = ZERO;
    for(int k = 0; k < small_numbers.size();k++){
      value = value.add(ONE);
      number_units[small_numbers[k] ] = value;
    }
    value = value.add(ONE);
    for(int k = 0; k < tens.size();k++){
      number_units[tens[k] ] = value;
      value = value.add(TEN);
    }
    
    
    //----------- Spanish ----------------------------
    initialize_es_hundreds();
    for (int k = 0; k < es_hundreds.size(); k++)
            number_units[es_hundreds[k] ] =  es_hundreds_val[k];
    
    number_units["un"] = ONE;
    
    number_units["cien"] = value;
    number_units["cientos"] = value;
    
    //accents stuff
    number_units["veintiseis"] = number_units["veintiséis"];
    number_units["veintidos"] = number_units["veintidós"];
    number_units["veintitres"] = number_units["veintitrés"];

    //number_units.put("millones",number_units.get("millón"));
    //number_units.put("billones",number_units.get("billón"));
    
    //Collections.addAll(nonbiglist,small_numbers);
    //Collections.addAll(nonbiglist,tens);
    nonbiglist.insert(nonbiglist.end(),small_numbers.begin(),small_numbers.end());
    nonbiglist.insert(nonbiglist.end(),tens.begin(),tens.end());
		      
    
    //Collections.addAll(nonbiglist,"un");
    //Collections.addAll(nonbiglist,"cien");
    //Collections.addAll(nonbiglist,"cientos");
    nonbiglist.push_back("un");
    nonbiglist.push_back("cien");
    nonbiglist.push_back("cientos");
    
    
    
    //accents AMIRITE
    //Collections.addAll(nonbiglist,"veintiseis");
    //Collections.addAll(nonbiglist,"veintidos");
    //Collections.addAll(nonbiglist,"veintitres");
    //Collections.addAll(nonbiglist,es_hundreds);

    nonbiglist.push_back("veintiseis");
    nonbiglist.push_back("veintidos");
    nonbiglist.push_back("veintitres");
    nonbiglist.insert(nonbiglist.end(),es_hundreds.begin(),es_hundreds.end());    
    
  }


  //wrapper for recursive function
  //returns zero if there is a mispelling
  bigint converter(std::string value){
    //string parsing
    std::string valuetosplit = ReplaceAll(value,"-"," ");
    std::transform(valuetosplit.begin(),valuetosplit.end(),valuetosplit.begin(), ::tolower);
    valuetosplit = ReplaceAll(valuetosplit," y "," ");
        //valuetosplit = valuetosplit.replace("mil ","mil-");
    valuetosplit = ReplaceAll(valuetosplit,","," "); // for text with commas.

        std::string::iterator new_end = std::unique(valuetosplit.begin(),valuetosplit.end(),
						[](char lhs,char rhs) ->
						bool {return (lhs==rhs) && (lhs == ' ');} );
    valuetosplit.erase(new_end,valuetosplit.end());

    valuetosplit = TRIM(valuetosplit);
    std::vector<std::string> splitvals = SPLIT( valuetosplit, " ");
    
    //------------------ error checks ------------------------------
    //std::string ones[] = Arrays.copyOfRange(small_numbers,0,9);

    std::vector<std::string> tenplusteens(small_numbers.begin()+9,small_numbers.end());
    //      []=Arrays.copyOfRange(small_numbers,9,small_numbers.size());
	
    std::string errorS = "Misspelling at word number ";
    std::string errorO = "Improper order at word number ";
    int k;
    bool rejoin = false;
    bool ordering=true;
	
    for(k = 0 ; k < splitvals.size();k++)
      {
	//TODO: combine some of these if statments and reduce the repeated boolean calculations
		if(number_units.find(splitvals[k])== number_units.end()){
	      //Fix the mil-siete type errors here
	      //arreglar errores como mil-siete
	      // This solution is not great but since most of the work is in BigInt operations the runtime
	      //shouldn't be greatly affected.
	      //std::string twovalues[] = splitvals[k].split("-");
	      //if(twovalues.size() == 2 && twovalues[0].equals("mil") &&
	      //   number_units.get(twovalues[1]) != null){
	      //splitvals[k] = twovalues[0] + " " + twovalues[1];
	      //rejoin = true;
                //}
                //else{
	      errorS += std::to_string(k+1)+": "+splitvals[k];
	      std::cout << errorS << std::endl;
	      //System.out.println(errorS);
	      return ZERO;
                //}
            }
      }
    
    //if(rejoin){
    //    std::string FUDGE = std::string.join(" ",splitvals);
    //    splitvals = FUDGE.split(" ");
    //}

    // check ordering:
    if(splitvals.size() > 1){
      for(k = 0 ; k < splitvals.size()-1;k++){
	if(number_units[splitvals[k] ].tostring().size() ==
	   number_units[splitvals[k+1] ].tostring().size() ){
	  ordering = false;
	      break;
	}
	
	if(number_units[splitvals[k] ].tostring().size() == 1 &&
	   (number_units[splitvals[k+1] ].tostring().size() == 2 ||
	    number_units[splitvals[k+1] ].tostring().size() == 3 )){
	  ordering = false;
	  break;
                }
	
	/*
	  number_units.put("veintiseis",number_units.get("veintiséis"));
	  number_units.put("veintidos",number_units.get("veintidós"));
	  number_units.put("veintitres",number_units.get("veintitrés"));
	*/
	
	if( ( (std::find(tenplusteens.begin(),tenplusteens.end(), splitvals[k]) != tenplusteens.end() ) ||
		 splitvals[k] == "veintiseis" ||
	     splitvals[k] == "veintidos" ||
	     splitvals[k] == "veintitres" ) &&
	    number_units[splitvals[k+1] ].tostring().size() <= 3 ){
	  ordering = false;
	  break;
	}
	    
	if( (std::find(tens.begin(),tens.end(),splitvals[k]) != tens.end() ) &&
	   number_units[splitvals[k+1] ].tostring().size() == 3 ){
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
	  // System.out.println(errorO);
	  return ZERO;
        }
	
        return conv_num(splitvals);
  }
  
  std::string reverse(bigint value){
    std::string numstr = value.tostring();
    int len = numstr.size();
    
    bigint HUNDRED = TEN;
    HUNDRED = HUNDRED.multiply(TEN);
    
    //bigint THOUSAND = HUNDRED.multiply(TEN);
    if(len <= 6){
      return reverse_conv_thousands(numstr);
    }
    
    int partitions = len / 6;
    int leftovers = len % 6;
    //System.out.println(leftovers);
    if(partitions <= 1 && leftovers == 0)
      return reverse_conv_thousands(numstr);
    
    std::string result = "";
    if(leftovers > 0)
      result += reverse_conv_thousands(numstr.substr(0,leftovers));
        if(leftovers != 0)
	  result += " "+numbers[--partitions];
        else
	  --partitions;
        int index = leftovers;
        while(partitions > 0){
	  result += " ";
	  result += reverse_conv_thousands(numstr.substr(index,6));
	  if(reverse_conv_thousands(numstr.substr(index,6)) != "")
	    result += " "+numbers[--partitions];
	  else
	    --partitions;
	  index += 6;
        }
	
        result += " ";
        result += reverse_conv_thousands(numstr.substr(index,numstr.size()));
        //fix double spaces here because of laziness.
        //std::string after = before.trim().replaceAll(" +", " ");
		std::string::iterator new_end = std::unique(result.begin(),result.end(),
						    [](char lhs,char rhs) ->
						    bool {return (lhs==rhs) && (lhs == ' ');} );
	result.erase(new_end,result.end());
	result = TRIM(result);


	//result = result.trim().replaceAll(" +"," ");
        //result = result.replace(" uno ","un ");
        // add the ' y ''s here
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
  std::vector<std::string> small_numbers = {"uno","dos","tres","cuatro","cinco",
					    "seis","siete","ocho","nueve","diez",
					    "once","doce","trece","catorce",
					    "quince","dieciseis","diecisiete",
					    "dieciocho","diecinueve","veinte",
					    "veintiuno","veintidós","veintitrés",
					    "veinticuatro","veinticinco",
					    "veintiséis","veintisiete",
					    "veintiocho","veintinueve"};


  
  std::vector<std::string> tens = {"treinta","cuarenta","cincuenta","sesenta","setenta","ochenta","noventa"};

  std::vector<std::string> es_hundreds = {"ciento","doscientos","trescientos","cuatrocientos",
					  "quinientos","seiscientos","setecientos",
					  "ochocientos","novecientos","mil"};

  std::vector<bigint> es_hundreds_val;
  bigint HUNDRED = bigint("100");
  void initialize_es_hundreds() {
    es_hundreds_val.resize(es_hundreds.size());
    es_hundreds_val[0] = HUNDRED;
    for (int k = 1; k < es_hundreds.size(); k++) {
      es_hundreds_val[k] = es_hundreds_val[k - 1].add(HUNDRED);
        }
  }

  std::vector<std::string> shortscale_names = {"millon",
					       "billon",
					       "trillon",
					       "cuatrillon",
					       "quintillon",
					       "sextillon",
					       "septillon",
					       "octillon",
					       "nonillon",
					       "decillon",
					       "undecillon",
					       "duodecillon",
					       "tredecillon",
					       "quattuordecillon",
					       "quindecillon",
					       "sexdecillon"};


  std::vector<std::string> numbers = {"millón",
				      "billón",
				      "trillón",
				      "cuatrillón",
				      "quintillón",
				      "sextillón",
				      "septillón",
				      "octillón",
				      "nonillón",
				      "decillón",
				      "undecillón",
				      "duodecillón",
				      "tredecillón",
				      "quattuordecillón",
				      "quindecillón",
				      "sexdecillón"};


  std::vector<bigint> numbervals;
  
  bigint THOUSAND = bigint("1000000");
  void initialize_numvalse() {
    numbervals.resize(numbers.size());
    numbervals[0] = THOUSAND;
    for (int k = 1; k < numbers.size(); k++) {
      numbervals[k] = numbervals[k - 1].multiply(THOUSAND);
    }
  }
  
  unordered_map<std::string,bigint> number_units;

    // THE CONVERTER
    //converter for numbers between 1 and 999
    //input and string array of length between 1 and 4;
    // TODO: make this recursive and throw exceptions when "hundred" is not in the right place.
  bigint convert_hundreds(std::vector<std::string> value){
    if(value.size()==1)
      return number_units[value[0]];
    else if(value.size()==2){
      return (number_units[value[0] ].add(number_units[value[1] ]) );
    }
    else if(value.size()==3)
      return ( (number_units[value[0] ].add(number_units[value[1] ] ))).add(number_units[value[2] ]);
    else {
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

  int find_mil(std::vector<std::string> values){
    int counter = -1;
    for(int k = 0; k < values.size();k++){
      if(values[k] == "mil"){
	counter = k;
                break;
      }
    }
    return counter;
  }

  bigint thousand = bigint("1000");
  bigint conv_small_text(std::vector<std::string> values){
    int s = find_first_big(values);
    if( std::find(values.begin(),values.end(),"mil") != values.end() ){
      if(values.size() ==1)
	return HUNDRED.multiply(TEN);
      int index = find_mil(values);
      //if(index == values.size() - 1){
      //System.out.println("Here");
      //	std::string arrayl[] = Arrays.copyOfRange(values,0,index);
      //
      //}
	  
      if(s<0){
	std::vector<std::string> arrayl(values.begin(),values.begin()+index);
	//= Arrays.copyOfRange(values,0,index);
	std::vector<std::string> arrayr(values.begin()+index+1,values.end());
	//[] = Arrays.copyOfRange(values,index+1,values.size());
	bigint result = number_units[values[index] ].multiply(convert_hundreds(arrayl));
	result = (index < values.size()-1)? result.add(convert_hundreds(arrayr)) : result.add(ZERO);
	return result;
      }
      else{
	std::vector<std::string> arrayl(values.begin(),values.begin()+s);
	//[] = Arrays.copyOfRange(values,0,s);
	std::vector<std::string> arrayr(values.begin()+s+1,values.end());
	//[] = Arrays.copyOfRange(values,s+1,values.size());
	bigint result = number_units[values[s] ].multiply(conv_small_text(arrayl));
	result = (index <= values.size()-1)? result.add(conv_small_text(arrayr)) : result.add(ZERO);
	//System.out.println("HERE");
	return result;
      }
    }
	
    if(s<0){
      return convert_hundreds(values);
    }
    else{
      std::vector<std::string> arrayl(values.begin(),values.begin()+s);
      //[] = Arrays.copyOfRange(values,0,s);
      std::vector<std::string> arrayr(values.begin()+s+1,values.end());
      //[] = Arrays.copyOfRange(values,s+1,values.size());
      bigint result = number_units[values[s] ].multiply(convert_hundreds(arrayl));
      result = (s < values.size()-1)? result.add(conv_small_text(arrayr)) : result.add(ZERO);
      return result;
    }
  }
  
  bigint conv_num(std::vector<std::string> values){
    //base cases
    if(values.size() <= 7){
      return conv_small_text(values);
    }
    
    int s = find_first_big(values);
    
    std::vector<std::string> arrayl(values.begin(),values.begin()+s);
    //[] = Arrays.copyOfRange(values,0,s);
    std::vector<std::string> arrayr(values.begin()+s+1,values.end());
    //[] = Arrays.copyOfRange(values,s+1,values.size());
    
    return (number_units[values[s] ].multiply(conv_small_text(arrayl))).add(conv_num(arrayr));
  }


  std::string reverse_conv_hundreds(std::string value){
    if(value == "000"){
      return "";
    } //there is so much bodging it's not even funny
    int intval = std::stoi(value);
    if(intval == 0){
            return "";
    }
    if(intval < 30)
      return small_numbers[intval-1];
    int hunds = intval / 100;
    int tenval=(intval % 100);
    int ones = (intval % 100) % 10;
    
    std::string result = "";
    if(hunds > 0)
            result += " " + es_hundreds[hunds-1] + " ";
    //result += small_numbers[hunds-1] + " hundred ";
    
    
        if(tenval != 0 && tenval < 30){
	  result += small_numbers[tenval-1];
        }
        if(tenval >= 30 && tenval <= 99){
	  result += tens[(tenval/10)-3]; //the three is there because it starts at 30.
	  if(tenval % 10 > 0){
	    result += " ";
	    result += small_numbers[(tenval%10) - 1];
	  }
        }
	
        return result;
	
  }
  
  std::string reverse_conv_thousands(std::string value){
    if(value == "000000"){
      return "";
        }
    if(value == "1000"){
      return " mil ";
    }
    
    //if(value.size() > 6){
    //  System.out.println(value);
    //}
    int intval = std::stoi(value);
    if(intval < 1000){
      return reverse_conv_hundreds(value);
    }
    std::string thouval = std::to_string(intval / 1000);
    std::string hundsval = std::to_string( intval % 1000);
    return reverse_conv_hundreds(thouval) + " mil " + reverse_conv_hundreds(hundsval);
	
  }
  
  //adding the y's function
  std::string addys(std::string result){

    result = ReplaceAll(result,"einta ","einta y ");
    result = ReplaceAll(result,"enta ","enta y ");
    result = ReplaceAll(result,"y mil","mil");

        for(int k = 0 ; k < shortscale_names.size(); k++){
	  result = ReplaceAll(result,"y "+ numbers[k],shortscale_names[k]+"es");
        }

        for(int k = 0 ; k < shortscale_names.size(); k++){
	  result = ReplaceAll(result," "+ numbers[k]," "+shortscale_names[k]+"es");
        }

        //	for(int k = 0 ; k < shortscale_names.size(); k++){
        //   result = result.replace("uno "+ shortscale_names[k]+"es","un "+numbers[k]);
        //}



        return result;
    }

};
