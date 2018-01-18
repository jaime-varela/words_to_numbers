import java.math.BigInteger;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.LinkedHashMap;


public class words_to_nums {
    words_to_nums() {
        initialize_numvalse();
        for (int k = 0; k < numbers.length; k++)
            number_units.put(numbers[k], numbervals[k]);

        BigInteger value = BigInteger.ZERO;
        for (int k = 0; k < small_numbers.length; k++) {
            value = value.add(BigInteger.ONE);
            number_units.put(small_numbers[k], value);
        }
        value = value.add(BigInteger.ONE);
        for (int k = 0; k < tens.length; k++) {
            number_units.put(tens[k], value);
            value = value.add(BigInteger.TEN);
        }
        number_units.put("hundred", value);

        Collections.addAll(nonbiglist, small_numbers);
        Collections.addAll(nonbiglist, tens);
        Collections.addAll(nonbiglist, "hundred");
    }


    //    public Integer
    private String small_numbers[] = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine",
            "ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen",
            "seventeen", "eighteen", "nineteen"};


    private String tens[] = {"twenty", "thirty", "forty", "fifty", "sixty", "seventy", "eighty", "ninety"};

    private String numbers[] = {"thousand", "million", "billion", "trillion", "quadrillion",
            "quintillion", "sextillion", "septillion", "octillion",
            "nonillion", "decillion", "undecillion", "duodecillion",
            "tredecillion", "quattuordecillion", "quindecillion",
            "sexdecillion", "septendecillion", "octodecillion",
            "novemdecillion", "vigintillion", "unvigintillion",
            "duovigintillion", "trevigintillion", "quatvigintillion",
            "quinvigintillion", "sexvigintillion", "septenvigintillion",
            "octovigintillion", "novemvigintillion", "trigintillion",
            "untrigintillion","duotrigintillion"};

    private BigInteger numbervals[] = new BigInteger[numbers.length];

    private BigInteger THOUSAND = new BigInteger("1000");

    private void initialize_numvalse() {
        numbervals[0] = THOUSAND;
        for (int k = 1; k < numbers.length; k++) {
            numbervals[k] = numbervals[k - 1].multiply(THOUSAND);
        }
    }

    private LinkedHashMap<String, BigInteger> number_units = new LinkedHashMap<String, BigInteger>();

    // THE CONVERTER
    //converter for numbers between 1 and 999
    //input and string array of length between 1 and 4;
    // TODO: make this recursive and throw exceptions when "hundred" is not in the right place.
    private BigInteger convert_hundreds(String[] value) {
        if (value.length == 1)
            return number_units.get(value[0]);
        else if (value.length == 2) {
            if (value[1].equals("hundred"))
                return (number_units.get(value[0]).multiply(number_units.get(value[1])));
            else
                return (number_units.get(value[0]).add(number_units.get(value[1])));
        } else if (value.length == 3)
            return ((number_units.get(value[0]).multiply(number_units.get(value[1])))).add(number_units.get(value[2]));
        else if (value.length == 4)
            return ((number_units.get(value[0]).multiply(number_units.get(value[1])))).add(number_units.get(value[2])).add(number_units.get(value[3]));
        else {
            System.out.println("improper number starting at "+value[0]+
			       " ending at "+value[value.length-1]);
            return BigInteger.ZERO;
        }
    }

    private ArrayList<String> nonbiglist = new ArrayList<>();

    // finds the first non small unit in string array, returns -1 other
    private int find_first_big(String[] values) {
        int counter = -1;
        for (int k = 0; k < values.length; k++) {
            if (!nonbiglist.contains(values[k])) {
                counter = k;
                break;
            }
        }
        return counter;
    }

    private BigInteger conv_small_text(String[] values) {
        int s = find_first_big(values);
        if (s < 0) {
            return convert_hundreds(values);
        } else {
            String arrayl[] = Arrays.copyOfRange(values, 0, s);
            String arrayr[] = Arrays.copyOfRange(values, s + 1, values.length);
            BigInteger result = number_units.get(values[s]).multiply(convert_hundreds(arrayl));
            result = (s < values.length - 1) ? result.add(conv_small_text(arrayr)) : result.add(BigInteger.ZERO);
            return result;
        }
    }

    private BigInteger conv_num(String[] values) {
        //base cases
        if (values.length <= 4) {
            return conv_small_text(values);
        }

        int s = find_first_big(values);

        String arrayl[] = Arrays.copyOfRange(values, 0, s);
        String arrayr[] = Arrays.copyOfRange(values, s + 1, values.length);

        return (number_units.get(values[s]).multiply(convert_hundreds(arrayl))).add(conv_num(arrayr));
    }

    //wrapper for recursive function
    //returns zero if there is a mispelling
    public BigInteger converter(String value) {
        //string parsing
        String valuetosplit = value.replace("-", " ").toLowerCase();
        valuetosplit = valuetosplit.replace(" and ", " ");
        valuetosplit = valuetosplit.replace(",", " "); // for text with commas.
        valuetosplit = valuetosplit.trim().replaceAll(" +"," "); //getting rid of double spaces
        String splitvals[] = valuetosplit.split(" ");

        String ones[] = Arrays.copyOfRange(small_numbers, 0, 9);
        String tenplusteens[] = Arrays.copyOfRange(small_numbers, 9, small_numbers.length);

        String errorS = "Misspelling at word number ";
        String errorO = "Improper order at word number ";
        int k;
        for (k = 0; k < splitvals.length; k++) {
            //TODO: combine some of these if statments and reduce the repeated boolean calculations
            if (number_units.get(splitvals[k]) == null) {
                errorS += Integer.toString(k + 1) + ": " + splitvals[k];
                System.out.println(errorS);
		return BigInteger.ZERO;
            }
            if (k < splitvals.length - 1 &&
                    Arrays.asList(tens).contains(splitvals[k]) && splitvals[k + 1].equals("hundred"))
                break;

            if (k < splitvals.length - 1 &&
                    Arrays.asList(small_numbers).contains(splitvals[k]) && Arrays.asList(tens).contains(splitvals[k + 1]))
                break;

            if (k < splitvals.length - 1 &&
                    Arrays.asList(tens).contains(splitvals[k]) && Arrays.asList(tenplusteens).contains(splitvals[k + 1]))
                break;

            if (k < splitvals.length - 1 &&
                    Arrays.asList(ones).contains(splitvals[k]) && Arrays.asList(tenplusteens).contains(splitvals[k + 1]))
                break;

            if (k < splitvals.length - 1 &&
                    Arrays.asList(tenplusteens).contains(splitvals[k]) && Arrays.asList(ones).contains(splitvals[k + 1]))
                break;

            if (k < splitvals.length - 1 &&
                    Arrays.asList(tenplusteens).contains(splitvals[k]) && splitvals[k + 1].equals("hundred"))
                break;
        }

        if (k < splitvals.length) {
            errorO += Integer.toString(k + 1) + ": " + splitvals[k] + " " + splitvals[k + 1];
            System.out.println(errorO);
            return BigInteger.ZERO;
        }

        return conv_num(splitvals);
    }

    private String reverse_conv_hundreds(String value) {
        if (value.equals("000")) {
            return "";
        }
        Integer intval = Integer.parseInt(value);
        if (intval < 20)
            return small_numbers[intval - 1];
        Integer hunds = intval / 100;
        Integer tenval = (intval % 100);
        Integer ones = (intval % 100) % 10;

        String result = "";
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


    public String reverse(BigInteger value) {
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
        if (partitions <= 1 && leftovers == 0)
            return reverse_conv_hundreds(numstr);

        String result = "";
        if (leftovers > 0)
            result += reverse_conv_hundreds(numstr.substring(0, leftovers));
        if (leftovers != 0)
            result += " " + numbers[--partitions];
        else
            --partitions;
        int index = leftovers;
        while (partitions > 0) {
            result += " ";
            result += reverse_conv_hundreds(numstr.substring(index, index + 3));
            if (!reverse_conv_hundreds(numstr.substring(index, index + 3)).equals(""))
                result += " " + numbers[--partitions];
            else
                --partitions;
            index += 3;
        }

        result += " ";
        result += reverse_conv_hundreds(numstr.substring(index, numstr.length()));
        //fix double spaces here because of laziness.
        //String after = before.trim().replaceAll(" +", " ");
        result = result.trim().replaceAll(" +", " ");

        //handle the and's here
        result = addys(result);
        return result;
    }

    private String addys(String result){
        result = result.replace("hundred ", "hundred and ");
        for(int k = 0; k < numbers.length;k++){
            result = result.replace("and "+numbers[k],numbers[k]);
        }

        for(int k = 0; k < numbers.length;k++){
            result = result.replace(" "+numbers[k]," "+numbers[k]+",");
        }

        if(result.substring(result.length() - 1).equals(","))
            result = result.substring(0,result.length()-1);
        return result;

    }

}
