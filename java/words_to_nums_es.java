import java.math.BigInteger;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.LinkedHashMap;

public class words_to_nums_es{
    words_to_nums_es(){
        initialize_numvalse();

        for (int k = 0; k < numbers.length; k++){
            number_units.put(numbers[k], numbervals[k]);
            number_units.put(numbers[k]+"es",numbervals[k]);
            number_units.put(shortscale_names[k], numbervals[k]);//unaccented
            number_units.put(shortscale_names[k]+"es", numbervals[k]);	//unaccented plurals
            //System.out.println(numbers[k] + "\t" + numbervals[k].toString());
            //	    if(k % 2 == 1){
            //	number_units.put(numbers[k]+"es",numbervals[k]);//plurals
            //	number_units.put(shortscale_names[k / 2], numbervals[k]);//unaccented
            //	number_units.put(shortscale_names[k / 2]+"es", numbervals[k]);	//unaccented plurals
            //}
        }
        BigInteger value = BigInteger.ZERO;
        for(int k = 0; k < small_numbers.length;k++){
            value = value.add(BigInteger.ONE);
            number_units.put(small_numbers[k],value);
        }
        value = value.add(BigInteger.ONE);
        for(int k = 0; k < tens.length;k++){
            number_units.put(tens[k],value);
            value = value.add(BigInteger.TEN);
        }


        //----------- Spanish ----------------------------
        initialize_es_hundreds();
        for (int k = 0; k < es_hundreds.length; k++)
            number_units.put(es_hundreds[k], es_hundreds_val[k]);

        number_units.put("un",BigInteger.ONE);

        number_units.put("cien",value);
        number_units.put("cientos",value);

        //accents stuff
        number_units.put("veintiseis",number_units.get("veintiséis"));
        number_units.put("veintidos",number_units.get("veintidós"));
        number_units.put("veintitres",number_units.get("veintitrés"));

        //number_units.put("millones",number_units.get("millón"));
        //number_units.put("billones",number_units.get("billón"));

        Collections.addAll(nonbiglist,small_numbers);
        Collections.addAll(nonbiglist,tens);

        Collections.addAll(nonbiglist,"un");
        Collections.addAll(nonbiglist,"cien");
        Collections.addAll(nonbiglist,"cientos");



        //accents AMIRITE
        Collections.addAll(nonbiglist,"veintiseis");
        Collections.addAll(nonbiglist,"veintidos");
        Collections.addAll(nonbiglist,"veintitres");
        Collections.addAll(nonbiglist,es_hundreds);

    }

    //    public Integer
    private String small_numbers[] = {"uno","dos","tres","cuatro","cinco",
            "seis","siete","ocho","nueve","diez",
            "once","doce","trece","catorce",
            "quince","dieciseis","diecisiete",
            "dieciocho","diecinueve","veinte",
            "veintiuno","veintidós","veintitrés",
            "veinticuatro","veinticinco",
            "veintiséis","veintisiete",
            "veintiocho","veintinueve"};



    private String tens[] = {"treinta","cuarenta","cincuenta","sesenta","setenta","ochenta","noventa"};

    private String es_hundreds[]={"ciento","doscientos","trescientos","cuatrocientos",
            "quinientos","seiscientos","setecientos",
            "ochocientos","novecientos","mil"};

    private BigInteger es_hundreds_val[]= new BigInteger[es_hundreds.length];
    private BigInteger HUNDRED = new BigInteger("100");
    private void initialize_es_hundreds() {
        es_hundreds_val[0] = HUNDRED;
        for (int k = 1; k < es_hundreds.length; k++) {
            es_hundreds_val[k] = es_hundreds_val[k - 1].add(HUNDRED);
        }
    }

    private String shortscale_names[] = {"millon",
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


    private String numbers[] = {"millón",
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


    private BigInteger numbervals[] = new BigInteger[numbers.length];

    private BigInteger THOUSAND = new BigInteger("1000000");
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
        else if(value.length==2){
            return (number_units.get(value[0]).add(number_units.get(value[1])) );
        }
        else if(value.length==3)
            return ( (number_units.get(value[0]).add(number_units.get(value[1]) ))).add(number_units.get(value[2]));
        else {
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

    private int find_mil(String[] values){
        int counter = -1;
        for(int k = 0; k < values.length;k++){
            if(values[k].equals("mil")){
                counter = k;
                break;
            }
        }
        return counter;
    }

    private BigInteger thousand = new BigInteger("1000");
    private BigInteger conv_small_text(String[] values){
        int s = find_first_big(values);
        if(Arrays.asList(values).contains("mil")){
            if(values.length ==1)
                return HUNDRED.multiply(BigInteger.TEN);
            int index = find_mil(values);
            //if(index == values.length - 1){
            //System.out.println("Here");
            //	String arrayl[] = Arrays.copyOfRange(values,0,index);
            //
            //}

            if(s<0){
                String arrayl[] = Arrays.copyOfRange(values,0,index);
                String arrayr[] = Arrays.copyOfRange(values,index+1,values.length);
                BigInteger result = number_units.get(values[index]).multiply(convert_hundreds(arrayl));
                result = (index < values.length-1)? result.add(convert_hundreds(arrayr)) : result.add(BigInteger.ZERO);
                return result;
            }
            else{
                String arrayl[] = Arrays.copyOfRange(values,0,s);
                String arrayr[] = Arrays.copyOfRange(values,s+1,values.length);
                BigInteger result = number_units.get(values[s]).multiply(conv_small_text(arrayl));
                result = (index <= values.length-1)? result.add(conv_small_text(arrayr)) : result.add(BigInteger.ZERO);
                //System.out.println("HERE");
                return result;
            }
        }

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
        if(values.length <= 7){
            return conv_small_text(values);
        }

        int s = find_first_big(values);

        String arrayl[] = Arrays.copyOfRange(values,0,s);
        String arrayr[] = Arrays.copyOfRange(values,s+1,values.length);

        return (number_units.get(values[s]).multiply(conv_small_text(arrayl))).add(conv_num(arrayr));
    }

    //wrapper for recursive function
    //returns zero if there is a mispelling
    public BigInteger converter(String value){
        //string parsing
        String valuetosplit = value.replace("-"," ").toLowerCase();
        valuetosplit = valuetosplit.replace(" y "," ");
        //valuetosplit = valuetosplit.replace("mil ","mil-");
        valuetosplit = valuetosplit.replace(","," "); // for text with commas.

        String splitvals[] = valuetosplit.split(" ");

        //------------------ error checks ------------------------------
        //String ones[] = Arrays.copyOfRange(small_numbers,0,9);
        String tenplusteens[]=Arrays.copyOfRange(small_numbers,9,small_numbers.length);

        String errorS = "Misspelling at word number ";
        String errorO = "Improper order at word number ";
        int k;
        boolean rejoin = false;
        boolean ordering=true;

        for(k = 0 ; k < splitvals.length;k++)
        {
            //TODO: combine some of these if statments and reduce the repeated boolean calculations
            if(number_units.get(splitvals[k])==null){
                //Fix the mil-siete type errors here
                //arreglar errores como mil-siete
                // This solution is not great but since most of the work is in BigInt operations the runtime
                //shouldn't be greatly affected.
                //String twovalues[] = splitvals[k].split("-");
                //if(twovalues.length == 2 && twovalues[0].equals("mil") &&
                //   number_units.get(twovalues[1]) != null){
                //splitvals[k] = twovalues[0] + " " + twovalues[1];
                //rejoin = true;
                //}
                //else{
                errorS += Integer.toString(k+1)+": "+splitvals[k];
                System.out.println(errorS);
		return BigInteger.ZERO;
                //}
            }
        }

        //if(rejoin){
        //    String FUDGE = String.join(" ",splitvals);
        //    splitvals = FUDGE.split(" ");
        //}

        // check ordering:
        if(splitvals.length > 1){
            for(k = 0 ; k < splitvals.length-1;k++){
                if(number_units.get(splitvals[k]).toString().length() ==
                        number_units.get(splitvals[k+1]).toString().length() ){
                    ordering = false;
                    break;
                }

                if(number_units.get(splitvals[k]).toString().length() == 1 &&
                        (number_units.get(splitvals[k+1]).toString().length() == 2 ||
                                number_units.get(splitvals[k+1]).toString().length() == 3 )){
                    ordering = false;
                    break;
                }

		/*
		  number_units.put("veintiseis",number_units.get("veintiséis"));
		  number_units.put("veintidos",number_units.get("veintidós"));
		  number_units.put("veintitres",number_units.get("veintitrés"));
		 */

                if( (Arrays.asList(tenplusteens).contains(splitvals[k]) ||
                        splitvals[k].equals("veintiseis") ||
                        splitvals[k].equals("veintidos") ||
                        splitvals[k].equals("veintitres") ) &&
                        number_units.get(splitvals[k+1]).toString().length() <= 3 ){
                    ordering = false;
                    break;
                }

                if(Arrays.asList(tens).contains(splitvals[k]) &&
                        number_units.get(splitvals[k+1]).toString().length() == 3 ){
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
        } //there is so much bodging it's not even funny
        Integer intval = Integer.parseInt(value);
        if(intval == 0){
            return "";
        }
        if(intval < 30)
            return small_numbers[intval-1];
        Integer hunds = intval / 100;
        Integer tenval=(intval % 100);
        Integer ones = (intval % 100) % 10;

        String result = "";
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

    public String reverse_conv_thousands(String value){
        if(value.equals("000000")){
            return "";
        }
        if(value.equals("1000")){
            return " mil ";
        }

        //if(value.length() > 6){
          //  System.out.println(value);
        //}
        Integer intval = Integer.parseInt(value);
        if(intval < 1000){
            return reverse_conv_hundreds(value);
        }
        String thouval = Integer.toString(intval / 1000);
        String hundsval = Integer.toString( intval % 1000);
        return reverse_conv_hundreds(thouval) + " mil " + reverse_conv_hundreds(hundsval);

    }

    public String reverse(BigInteger value){
        String numstr = value.toString();
        Integer len = numstr.length();

        BigInteger HUNDRED = BigInteger.TEN;
        HUNDRED = HUNDRED.multiply(BigInteger.TEN);

        //BigInteger THOUSAND = HUNDRED.multiply(BigInteger.TEN);
        if(len <= 6){
            return reverse_conv_thousands(numstr);
        }

        Integer partitions = len / 6;
        Integer leftovers = len % 6;
        //System.out.println(leftovers);
        if(partitions <= 1 && leftovers == 0)
            return reverse_conv_thousands(numstr);

        String result = "";
        if(leftovers > 0)
            result += reverse_conv_thousands(numstr.substring(0,leftovers));
        if(leftovers != 0)
            result += " "+numbers[--partitions];
        else
            --partitions;
        int index = leftovers;
        while(partitions > 0){
            result += " ";
            result += reverse_conv_thousands(numstr.substring(index,index+6));
            if(!reverse_conv_thousands(numstr.substring(index,index+6)).equals(""))
                result += " "+numbers[--partitions];
            else
                --partitions;
            index += 6;
        }

        result += " ";
        result += reverse_conv_thousands(numstr.substring(index,numstr.length()));
        //fix double spaces here because of laziness.
        //String after = before.trim().replaceAll(" +", " ");
        result = result.trim().replaceAll(" +"," ");
        //result = result.replace(" uno ","un ");
        // add the ' y ''s here
        result = addys(result);

        return result;
    }

    //adding the y's function
    private String addys(String result){

        result = result.replace("einta ","einta y ");
        result = result.replace("enta ","enta y ");
        result = result.replace("y mil","mil");

        for(int k = 0 ; k < shortscale_names.length; k++){
            result = result.replace("y "+ numbers[k],shortscale_names[k]+"es");
        }

        for(int k = 0 ; k < shortscale_names.length; k++){
            result = result.replace(" "+ numbers[k]," "+shortscale_names[k]+"es");
        }

        //	for(int k = 0 ; k < shortscale_names.length; k++){
        //   result = result.replace("uno "+ shortscale_names[k]+"es","un "+numbers[k]);
        //}



        return result;
    }

}
