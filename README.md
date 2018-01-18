# words_to_numbers
Words to Numbers to Words in either english, spanish, or french using Java or C++.

The files in the folders are stand alone classes that can be used to convert a number in words to its numerical values or 
vice versa.  Some of the code here is used in the application "Words to Numbers" found here

https://play.google.com/store/apps/details?id=com.dahak.wordstonumbers


The java code has been tested the most while the c++ has been tested least, if there are bugs please let me know.

The only public methods for the java classes are:

BigInteger converter(String)
String reverse(BigInteger)

Here, "converter" takes a string and outputs the corresponding BigInteger and "reverse" does the opposite.

The methods for the C++ files are the same except with std::string and bigint replacing String and BigInteger, respectively.


