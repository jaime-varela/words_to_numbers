import java.math.*;
import java.util.LinkedHashMap;
import java.util.Arrays;
import java.util.ArrayList;
import java.util.Collections;
import java.math.BigInteger;
//import java.text.NumberFormat;
//used with bigints

import java.util.Random;

public class simple_test{

    public static void main(String[] args){
	
	//-------------- TEST ---------------------------------
	boolean fail = false;

	
	 
	Random rand = new Random();
	words_to_nums en_test = new words_to_nums();
	//words_to_nums_fr fr_test = new words_to_nums_fr();
	//words_to_nums_es es_test = new words_to_nums_es();
	for(int k= 0; k< 10000;k++){///////////
	    Integer S = rand.nextInt(10000)+1;
	    BigInteger Sbigint = new BigInteger(S.toString());

	    //BigInteger Sbigint = new BigInteger(256/2, rand);

	    //System.out.println(S);
	    
	    String value = en_test.reverse(Sbigint);
	    //String value = fr_test.reverse(Sbigint);
	    //String value = es_test.reverse(Sbigint);

	    BigInteger F = en_test.converter(value);
	    //BigInteger F = fr_test.converter(value);
	    //BigInteger F = es_test.converter(value.trim().replaceAll(" +"," "));

	    if(Sbigint.compareTo(F)!=0)
		fail = true;
	    if(fail){
		System.out.println(Sbigint);
		System.out.println("converter failed with: "+ value);
		System.out.println(F);
		fail = false;
	    }
	    //if(k < 10)
	    //System.out.println(Sbigint);
	}

    }
}
