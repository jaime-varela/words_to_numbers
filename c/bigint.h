#ifndef MY_BIGINT
#define MY_BIGINT

#include <stdlib.h>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

class bigint
{
public:

  //-----------------------------------
  //--------- Constructors  -----------
  //-----------------------------------
  bigint() : number(1,0), sign{1} {}
  bigint(long long val){
    *this = val;
  }
  bigint(const bigint& val){
    sign = val.sign;
    number = val.number;
  }
  bigint(const std::string& val);
  void operator=(const bigint & val){
    sign = val.sign;
    number = val.number;    
  }
  void operator=(long long val){
    sign = (val < 0)? -1 : 1;
    val = (val < 0)? -val: val;
    for(; val > 0; val = val / base)
      number.push_back(val % base);
  }
  
  //-----------------------------------
  //--- Addition and subtraction ------
  //-----------------------------------
  
  bigint operator+(const bigint & val) const{
    int valen = val.number.size();
    int len = number.size();
    if(sign == val.sign){
      bigint result = val;
      for(int i=0, carry=0;
	  i < (int) std::max(valen,len) || carry; ++i){
	if(i == result.length())
	  result.number.push_back(0);
	result.number[i] += carry + (i < (int) len ? number[i] : 0);
	carry = result.number[i] >= base;
	if(carry)
	  result.number[i] -= base;
      }
      return result;
    }
    return *this - (-val);
  }

  bigint operator-(const bigint & val) const {
    int valen = val.number.size();
    if(sign == val.sign){
      if((*this).mag() >= val.mag()) {
	bigint result = *this;
	for (int i = 0, carry = 0; i < valen || carry; ++i) {
	  result.number[i] -= carry + (i < valen ? val.number[i] : 0);
	  carry = result.number[i] < 0;
	  if (carry)
	    result.number[i] += base;
	}
	result.remzeros();
	return result;
      }
      return -(val - *this);
    }
    return *this + (-val);
  }
  
  bigint operator-() const{
    bigint result = *this;
    result.sign = -sign;
    return result;
  }

  void operator+=(const bigint & val){
    *this = *this + val;
  }
  void operator+=(int val){
    bigint v = bigint(val);
    *this = *this + v;
  }
  void operator-=(const bigint & val){
    *this = *this - val;
  }
  void operator-=(int val){
    bigint v = bigint(val);
    *this = *this - v;
  }
  //-----------------------------------
  //--------- Booleans ----------------
  //-----------------------------------
  bool operator<(const bigint & val) const{
    if(sign != val.sign)
      return sign < val.sign;
    if(number.size() != val.length())
      return number.size() * sign < val.length() * val.sign;
    for(int i = number.size() - 1; i >=0; i--)
      if(number[i] != val.number[i])
	return number[i] * sign < val.number[i] * sign;
    return false;
  }
  bool operator>(const bigint & val) const {
    return val < *this;
  }
  bool operator<=(const bigint & val) const{
    return !(val < *this);
  }
  bool operator >=(const bigint & val) const{
    return !(*this < val);
  }
  bool operator==(const bigint & val) const {
    return !(*this < val) && !(val < *this);
  }
  bool operator!=(const bigint & val) const {
    return *this < val || val < *this;
  }
    
  //-----------------------------------
  //--------- Multiplication ----------
  //-----------------------------------

  //use katsubara for now.
  bigint operator*(const bigint & rhs)
  {
    bigint result;
    result = karatsuba(*this,rhs);
    if(result.sign != this->sign * rhs.sign)
      result.sign = this->sign * rhs.sign;
    return result;
  }  
  bigint operator*(int rhs)
  {
  bigint RHS(rhs);
  return *this * RHS;
  }
  //assumes positive values only
  bigint schoolboy_mult(const bigint & lhs,
		   const bigint & rhs){
    bigint product;
    int lhslen = lhs.length();
    int  rhslen = rhs.length();
    long long prod = 0;
    product.number.resize(lhslen+rhslen);
    int carry = 0;
    for(int bi = 0; bi < rhslen;bi++){
      carry = 0;
      for(int ai = 0; ai < lhslen;ai++){
	prod = (long long) product.number[ai+bi] + carry +
	  ((long long) lhs.number[ai]) * ((long long) rhs.number[bi]);
	carry = (int) (prod / base);
	product.number[ai+bi] = (int) (prod % base);
      }
      product.number[bi + (lhslen)] += carry;
    }
    product.remzeros();
    return product;
  } 
  //assumes positive values only
  bigint karatsuba(const bigint & lhs,
		   const bigint & rhs){
    bigint zero(0);
    if(lhs == zero || rhs == zero)
      return zero;
    if(lhs.length() < 11 || rhs.length() < 11)
      return schoolboy_mult(lhs,rhs);
    else{
      std::string lhsstr = lhs.tostring();
      std::string rhsstr = rhs.tostring();
      int m,m2;
      m = std::max(lhsstr.size(),rhsstr.size());
      m2 = m/2;
      //std::cout << m2 << std::endl;
      bigint x1,y1,x0,y0, z2,z0,z1;
      // handle the number splits here
      std::string x1str,y1str,x0str,y0str;
      x0str = (lhsstr.size() >= m2)? lhsstr.substr(lhsstr.size() - m2): lhsstr;
      y0str = (rhsstr.size() >= m2)? rhsstr.substr(rhsstr.size() - m2): rhsstr;

      x1str = (lhsstr.size() <= m2)? "0": lhsstr.substr(0,lhsstr.size()-m2);
      y1str = (rhsstr.size() <= m2)? "0": rhsstr.substr(0,rhsstr.size()-m2);

      x1=bigint(x1str);
      x0=bigint(x0str);
      y1=bigint(y1str);
      y0=bigint(y0str);
      //      std::cout << lhs
      z2 = karatsuba(x1,y1);
      z0 = karatsuba(x0,y0);
      z1 = karatsuba((x1+x0),(y1+y0))-z2-z0;
      bigint result;
      result = z2.pow10(2*m2)+ z1.pow10(m2) + z0;
      return result;      
    }    
  }

  void operator*=(const bigint &v) {
    *this = *this * v;
  }
  void operator*=(int  val){
    bigint v = bigint(val);
    *this = *this * v;
  }
    
  //-----------------------------------
  //--------- Division ----------------
  //-----------------------------------
  bigint operator/(const bigint & v) const {
    return divmod(*this, v).first;
  }
  bigint operator%(const bigint &v) const {
    return divmod(*this, v).second;
  }  
  //TODO: use a different algorithm
  std::pair<bigint, bigint> divmod(const bigint & num,
				   const bigint & div) const{
    bigint zero("0");
    int norm = base / (div.number.back() + 1);
    bigint a = num.mag() * norm;
    bigint b = div.mag() * norm;
    bigint q, r;
    q.number.resize(a.number.size());

    for(int i = a.number.size() - 1; i >=0;i--){
      r *= base;
      r += a.number[i];
      int S1 = r.number.size() <= b.number.size() ? 0 : r.number[b.number.size()];
      int S2 = r.number.size() <= b.number.size() - 1 ? 0 : r.number[b.number.size() - 1];
      int d = ((long long) base * S1 + S2) / b.number.back();
      r -= b * d;
      while(r < zero)
	r += b, --d;
      q.number[i] = d;
    }
    q.sign = num.sign * div.sign;
    r.sign = num.sign;
    q.remzeros();
    r.remzeros();
    return std::make_pair(q, r / norm);
  }
  void operator/=(int v) {
    if (v < 0)
      sign = -sign, v = -v;
    for (int i = (int) number.size() - 1, rem = 0; i >= 0; --i) {
      long long cur = number[i] + rem * (long long) base;
      number[i] = (int) (cur / v);
      rem = (int) (cur % v);
    }
    remzeros();
  }
  bigint operator/(int v) const {
    bigint res = *this;
    res /= v;
    return res;
  }
  int operator%(int v) const {
    if (v < 0)
      v = -v;
    int m = 0;
    for (int i = number.size() - 1; i >= 0; --i)
      m = (number[i] + m * (long long) base) % v;
    return m * sign;
  }
  void operator/=(const bigint & val){
    *this = *this / val;
  }
  //-----------------------------------
  //--------- utility / misc ----------
  //-----------------------------------
    
  bigint mag() const {
    bigint result = *this;
    result.sign *= result.sign;
    return result;
  }

  bool iszero() const {
    return number.empty() || (number.size() == 1 && !number[0]);
  }

  int sign;
  std::vector<int> number;
  int length() const {return number.size();}
  //removes leading zeros in the int array
  void remzeros(){
    while(!number.empty() && !number.back())
      number.pop_back();
    if(number.empty())
      sign = 1;
  }

  bigint pow10(int n){
    int nadd = n;
    if(nadd > base_digits){
      std::reverse(number.begin(),number.end());
      while(nadd > base_digits){
	number.push_back(0);
	nadd -= base_digits;
      }
      std::reverse(number.begin(),number.end());
    }    
    *this = schoolboy_mult(*this,
			   powof10(nadd));
    return *this;
  }

  bigint powof10(int n){
    int val = 10;
    while(n > 1){
      val *= 10;
      n--;
    }
    return bigint((long long) val);
  }

  friend std::ostream & operator<<(std::ostream & out,
				   const bigint & rhs)  {
    out << rhs.tostring();
    return out;
  }

  std::string tostring() const{
    bigint v = *this;
    std::string stream = "";
    if (v.sign == -1)
      stream += "-";
    stream += (v.number.empty() ? std::to_string(0) : std::to_string(v.number.back()) );
    std::string mid = "";
    for (int i = (int) v.number.size() - 2; i >= 0; --i){
      mid = std::to_string(v.number[i]);
      while(mid.length() < base_digits)
	mid = "0"+mid;
      stream += mid;
      //stream << setw(base_digits) << setfill('0') << v.a[i];
    }	
    return stream;    
  }

  bigint ZERO(){
    bigint zero = bigint("0");
    return zero;
  }
  bigint TEN(){
    bigint ten = bigint("10");
    return ten;
  }
  bigint ONE(){
    bigint one = bigint("1");
    return one;
  }
  bigint add(bigint val){
    return *this + val;
  }
  bigint multiply(bigint val){
    return ( *this) * val;
  }

  friend bigint gcd(const bigint & a, const bigint & b) {
    return b.iszero() ? a : gcd(b, a % b);
  }
  friend bigint lcm(const bigint & a, const bigint & b) {
    return a / gcd(a, b) * b;
  }

  
private:
  int base = 1000000000;
  int base_digits = 9;      
};

//string constructor
bigint::bigint(const std::string& val){
  int len = val.size(), value,index = 0, length;  
  sign = 1;
  std::string val2 = val;
  if(val2[0]=='-'){
    sign = -1;
    val2 = val.substr(1);
  }
  if(val2.size() <= base_digits){
    number.push_back(std::stoi(val2));
  }
  else{
    while(len > 0){
      index = std::max(0,len-base_digits);
      length = (len-base_digits < 0)? (len): base_digits;
      value = std::stoi(val2.substr(index,length));
      number.push_back(value);
      len -= base_digits;
    }    
  }
}

bigint operator*(int lhs,bigint & rhs)
{
  return rhs * lhs;  
}  

#endif // MY_BIGINT
