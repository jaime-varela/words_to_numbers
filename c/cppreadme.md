In order for this program to work a suitable big int library must be used.  I used the one found here

https://sites.google.com/site/indy256/algo_cpp/bigint

I could find no liscencing information so if you plan to use this program then I would suggest implementing your own c++ 
bigint library of choice and replacing the "bigint" with your own class/object.  The only requirements is that the big integer
object must have the accessible methods "add","multiply", and "tostring()".  In other words if A is your big integer 
object and B is another object then A.add(B) returns A+B.

I plan to replace the current bigint library with my own at some point.

