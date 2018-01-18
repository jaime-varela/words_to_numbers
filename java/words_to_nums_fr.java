import java.math.BigInteger;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.LinkedHashMap;

public class words_to_nums_fr{
    words_to_nums_fr(){
        initialize_numvalse();

        for (int k = 0; k < numbers.length; k++){
            number_units.put(numbers[k], numbervals[k]);
            if(k>0)//plurals
                number_units.put(numbers[k]+"s", numbervals[k]);
        }
        BigInteger value = BigInteger.ZERO;
        for(int k = 0; k < small_numbers.length;k++){
            value = value.add(BigInteger.ONE);
            number_units.put(small_numbers[k],value);
        }

        Collections.addAll(nonbiglist,small_numbers);
    }

    //    public Integer
    private String small_numbers[] = {"un",
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




    private String numbers[] = {"mille",
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


    private BigInteger numbervals[] = new BigInteger[numbers.length];

    private BigInteger THOUSAND = new BigInteger("1000");
    private void initialize_numvalse() {
        numbervals[0] = THOUSAND;
        for (int k = 1; k < numbers.length; k++) {
            numbervals[k] = numbervals[k - 1].multiply(THOUSAND);
        }
    }

    private LinkedHashMap<String,BigInteger> number_units = new LinkedHashMap<String,BigInteger>();

    // THE CONVERTER
    //converter for numbers between 1 and 999
    //input and string array of length between 1 and 4;
    // TODO: make this recursive and throw exceptions when "hundred" is not in the right place.
    private BigInteger convert_hundreds(String[] value){
        if(value.length==1)
            return number_units.get(value[0]);
        else if(value.length==2 && value[0].equals("cent")){
            return (number_units.get(value[0]).add(number_units.get(value[1])) );
        }
        else if(value.length==2 && value[1].equals("cent")){
            return (number_units.get(value[0]).multiply(number_units.get(value[1])) );
        }
        else if(value.length==3 && value[1].equals("cent"))
            return ( (number_units.get(value[0]).multiply(number_units.get(value[1]) ))).add(number_units.get(value[2]));
        else{
            System.out.println("improper number starting at "+value[0]+
			       " ending at "+value[value.length-1])
            return BigInteger.ZERO;
        }
    }

    private ArrayList<String> nonbiglist = new ArrayList<>();
    // finds the first non small unit in string array, returns -1 other
    private int find_first_big(String[] values){
        int counter = -1;
        for(int k = 0; k < values.length;k++){
            if(!nonbiglist.contains(values[k])){
                counter = k;
                break;
            }
        }
        return counter;
    }

    private BigInteger conv_small_text(String[] values){
        int s = find_first_big(values);
        if(s<0){
            return convert_hundreds(values);
        }
        else{
            String arrayl[] = Arrays.copyOfRange(values,0,s);
            String arrayr[] = Arrays.copyOfRange(values,s+1,values.length);
            BigInteger result = number_units.get(values[s]).multiply(convert_hundreds(arrayl));
            result = (s < values.length-1)? result.add(conv_small_text(arrayr)) : result.add(BigInteger.ZERO);
            return result;
        }
    }

    private BigInteger conv_num(String[] values){
        //base cases
        if(values.length <= 4){
            return conv_small_text(values);
        }

        int s = find_first_big(values);

        String arrayl[] = Arrays.copyOfRange(values,0,s);
        String arrayr[] = Arrays.copyOfRange(values,s+1,values.length);

        return (number_units.get(values[s]).multiply(convert_hundreds(arrayl))).add(conv_num(arrayr));
    }

    //wrapper for recursive function
    //returns zero if there is a mispelling
    public BigInteger converter(String value){
        //string parsing
        String valuetosplit = value.toLowerCase();
        valuetosplit = valuetosplit.replace(" et ","-et-");
        valuetosplit = valuetosplit.replace(","," "); // for text with commas.

        String splitvals[] = valuetosplit.split(" ");

        //------------------ error checks ------------------------------
        //String ones[] = Arrays.copyOfRange(small_numbers,0,9);
        //String tenplusteens[]=Arrays.copyOfRange(small_numbers,9,small_numbers.length);

        String errorS = "Misspelling at word number ";
        String errorO = "Improper order at word number ";
        int k;

        boolean ordering=true;

        for(k = 0 ; k < splitvals.length;k++)
        {
            //TODO: combine some of these if statments and reduce the repeated boolean calculations
            if(number_units.get(splitvals[k])==null){
                errorS += Integer.toString(k+1)+": "+splitvals[k];
                System.out.println(errorS);
		return BigInteger.ZERO;
            }

        }

        // check ordering (not complete):
        if(splitvals.length > 1){
            for(k = 0 ; k < splitvals.length-1;k++){
                if(number_units.get(splitvals[k]).toString().length() ==
                        number_units.get(splitvals[k+1]).toString().length() ){
                    ordering = false;
                    break;
                }

                if(number_units.get(splitvals[k]).toString().length() == 1 &&
                        (number_units.get(splitvals[k+1]).toString().length() == 2)){
                    ordering = false;
                    break;
                }

                if(number_units.get(splitvals[k]).toString().length() == 2 &&
                        (number_units.get(splitvals[k+1]).toString().length() == 3)){
                    ordering = false;
                    break;
                }

            }
        }
        k++;
        //System.out.println(k);
        if(!ordering){
            errorO += Integer.toString(k)+": "+splitvals[k-1]+" "+splitvals[k];
            System.out.println(errorO);
	    return BigInteger.ZERO;
        }


        return conv_num(splitvals);
    }


    private String reverse_conv_hundreds(String value){
        if(value.equals("000")){
            return "";
        }
        Integer intval = Integer.parseInt(value);
        //System.out.println(value);
        if(intval <= 100)
            return small_numbers[intval-1];
        Integer hunds = intval / 100;
        Integer tens = intval % 100;

        //System.out.println(tens-1);
        String result = "";
        result += small_numbers[hunds-1];
        result += " cent ";
        if(tens > 0)
            result += small_numbers[tens-1];
        return result;
        //return small_numbers[hunds - 1] +" cent "+ small_numbers[tens-1];
    }


    public String reverse(BigInteger value){
        String numstr = value.toString();
        Integer len = numstr.length();

        BigInteger HUNDRED = BigInteger.TEN;
        HUNDRED = HUNDRED.multiply(BigInteger.TEN);

        if(value.compareTo(HUNDRED)<0){
            return reverse_conv_hundreds(numstr);
        }

        Integer partitions = len / 3;
        Integer leftovers = len % 3;
        //System.out.println(leftovers);
        if(partitions <= 1 && leftovers == 0)
            return reverse_conv_hundreds(numstr);

        String result = "";
        if(leftovers > 0)
            result += reverse_conv_hundreds(numstr.substring(0,leftovers));
        if(leftovers != 0)
            result += " "+numbers[--partitions];
        else
            --partitions;
        int index = leftovers;
        while(partitions > 0){
            result += " ";
            result += reverse_conv_hundreds(numstr.substring(index,index+3));
            if(!reverse_conv_hundreds(numstr.substring(index,index+3)).equals(""))
                result += " "+numbers[--partitions];
            else
                --partitions;
            index += 3;
        }

        result += " ";
        result += reverse_conv_hundreds(numstr.substring(index,numstr.length()));
        //fix double spaces here because of laziness.
        //String after = before.trim().replaceAll(" +", " ");
        result = result.trim().replaceAll(" +"," ");
        return result;
    }

}
