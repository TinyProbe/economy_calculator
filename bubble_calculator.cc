#include <algorithm>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <cstdlib>
using namespace std;
int argument_check(int ac,
                   char **av,
                   double &principal,
                   double &reserve_ratio,
                   double &interest_rate) {
  if (ac != 4) {
    cerr << "invaild argument:" << '\n';
    cerr << "  usage:" << '\n';
    cerr << "    ./[a.exe] <principal> <reserve_ratio> <interest_rate>" << '\n';
    cerr << "    <principal>: Principal held in a bank." << '\n';
    cerr << "    <reserve_ratio>: Bank's reserve ratio." << '\n';
    cerr << "    <interest_rate>: Annual interest rate." << '\n';
    return -1;
  }
  principal = atof(av[1]);
  reserve_ratio = atof(av[2]);
  interest_rate = atof(av[3]);
  if (principal < 1e0 || principal > 1e100) {
    cerr << "invaild argument:" << '\n';
    cerr << "  principal:" << '\n';
    cerr << "    1e0 <= principal && principal <= 1e100" << '\n';
    return -2;
  }
  if (reserve_ratio < 1e-5 || reserve_ratio > 1e2) {
    cerr << "invaild argument:" << '\n';
    cerr << "  reserve_ratio:" << '\n';
    cerr << "    1e-5 <= reserve_ratio && reserve_ratio <= 1e2" << '\n';
    return -3;
  }
  if (interest_rate < 1e-5 || interest_rate > 1e100) {
    cerr << "invaild argument:" << '\n';
    cerr << "  interest_rate:" << '\n';
    cerr << "    1e-5 <= interest_rate && interest_rate <= 1e100" << '\n';
    return -4;
  }
  return 0;
}
double bubble_calculate(double const &principal,
                        double const &reserve_ratio) {
  return 1e2 / reserve_ratio * principal;
}
double inflation_calculate(double const &principal,
                           double const &interest_rate,
                           double const &actual_circulation) {
  return (actual_circulation - principal) * (interest_rate / 1e2);
}
string const &regex_money(double value, size_t unit) {
  static string s1, s2;
  stringstream ss;
  ss << fixed << setprecision(0) << value;
  ss >> s1;
  s2.clear();
  for (size_t i = 1; i <= s1.size(); ++i) {
    if (i % unit == 1) { s2 += ','; }
    s2 += s1[s1.size() - i];
  }
  reverse(s2.begin(), s2.end());
  s2.pop_back();
  return s2;
}
int main(int ac, char **av) {
  ios::sync_with_stdio(0), cin.tie(0), cout.tie(0);
  double principal;
  double reserve_ratio;
  double interest_rate;
  double actual_circulation;
  double inflation;
  size_t unit = 3;
  if (int tmp = argument_check(ac, av, principal, reserve_ratio, interest_rate))
    return tmp;
  actual_circulation = bubble_calculate(principal, reserve_ratio);
  inflation = inflation_calculate(principal, interest_rate, actual_circulation);
  cout << "Actual circulation: " <<
      regex_money(actual_circulation, unit) << '\n';
  cout << "Inflation(year): " <<
      regex_money(inflation, unit) << '\n';
  return 0;
}
