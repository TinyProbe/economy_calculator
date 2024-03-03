#include <algorithm>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>
#include <random>
using namespace std;
int argument_check(int ac,
                   char **av,
                   long long &n,
                   long long &income,
                   double &tax_rate,
                   double &tax_rate_l) {
  if (ac < 4 || ac > 5) {
    cerr << "invaild argument:" << '\n';
    cerr << "  usage:" << '\n';
    cerr << "    ./[a.exe] <n> <income> <tax_rate> [tax_rate_l]" << '\n';
    cerr << "    <n>: Number of social members." << '\n';
    cerr << "    <income>: Base income(year)." << '\n';
    cerr << "    <tax_rate>(%): Base tax rate." << '\n';
    cerr << "    [tax_rate_l]: Limit tax rate." << '\n';
    return -1;
  }
  n = atoll(av[1]);
  income = atoll(av[2]);
  tax_rate_l = tax_rate = atof(av[3]);
  if (ac == 5) {
    tax_rate_l = atof(av[4]);
  }
  if (n < (long long)1e0 || n > (long long)1e8) {
    cerr << "invaild argument:" << '\n';
    cerr << "  n:" << '\n';
    cerr << "    1e0 <= n && n <= 1e8" << '\n';
    return -2;
  }
  if (income < (long long)1e0 || income > (long long)1e12) {
    cerr << "invaild argument:" << '\n';
    cerr << "  income:" << '\n';
    cerr << "    1e0 <= income && income <= 1e12" << '\n';
    return -3;
  }
  if (tax_rate < 1e-5 || tax_rate > 1e5) {
    cerr << "invaild argument:" << '\n';
    cerr << "  tax_rate(%):" << '\n';
    cerr << "    1e-5 <= tax_rate && tax_rate <= 1e5" << '\n';
    return -4;
  }
  if (tax_rate_l < 1e-5 || tax_rate_l > 1e5) {
    cerr << "invaild argument:" << '\n';
    cerr << "  tax_rate_l(%):" << '\n';
    cerr << "    1e-5 <= tax_rate_l && tax_rate_l <= 1e5" << '\n';
    return -5;
  }
  if (tax_rate > tax_rate_l) {
    cerr << "invaild argument:" << '\n';
    cerr << "  tax_rate, tax_rate_l:" << '\n';
    cerr << "    tax_rate <= tax_rate_l" << '\n';
    return -6;
  }
  return 0;
}
template<class T> T __random(T mnm, T mxm) {
  static mt19937 gen((random_device())());
  return (uniform_int_distribution<T>(mnm, mxm))(gen);
}
void generate_random(vector<long long> &property, long long n) {
  size_t const sect = 7;
  long long div = n / sect;
  long long rmn = n % sect;
  long long val = 100'000;
  size_t ix = 0;
  property.resize(n);
  for (size_t i = 0; i < sect; ++i) {
    for (size_t j = 0; j < div + (rmn > i); ++j) {
      property[ix++] = __random(val, val * 10);
    }
    val *= 10;
  }
}
void tax_rate_calculate_before(vector<long long> const &property,
                               vector<long long> &before) {
  size_t n = property.size();
  before.resize(n);
  for (size_t i = 0; i < n; ++i) {
    if (property[i] <= 12'000'000) {
      before[i] = property[i] * 0.06;
    } else if (property[i] <= 46'000'000) {
      before[i] = property[i] * 0.15 - 1'080'000;
    } else if (property[i] <= 88'000'000) {
      before[i] = property[i] * 0.24 - 5'220'000;
    } else if (property[i] <= 150'000'000) {
      before[i] = property[i] * 0.35 - 14'900'000;
    } else if (property[i] <= 300'000'000) {
      before[i] = property[i] * 0.38 - 19'400'000;
    } else if (property[i] <= 500'000'000) {
      before[i] = property[i] * 0.40 - 25'400'000;
    } else if (property[i] <= 1'000'000'000) {
      before[i] = property[i] * 0.42 - 35'400'000;
    } else {
      before[i] = property[i] * 0.45 - 65'400'000;
    }
  }
}
void tax_rate_calculate_after(vector<long long> const &property,
                              vector<long long> &after,
                              long long income,
                              double tax_rate,
                              double tax_rate_l) {
  size_t n = property.size();
  after.resize(n);
  tax_rate /= 1e2;
  tax_rate_l /= 1e2;
  for (size_t i = 0; i < n; ++i) {
    double ratio = (double)property[i] / (double)income;
    after[i] = min(tax_rate_l, tax_rate * ratio) * property[i];
  }
}
template<class T> string const &regex_money(T value, size_t unit) {
  static string s1, s2;
  stringstream ss;
  ss << fixed << setprecision(20) << value;
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
  vector<long long> property, before, after;
  long long n, income;
  double tax_rate, tax_rate_l;
  size_t unit = 3;
  if (int tmp = argument_check(ac, av, n, income, tax_rate, tax_rate_l)) {
    return tmp;
  }
  generate_random(property, n);
  tax_rate_calculate_before(property, before);
  tax_rate_calculate_after(property, after, income, tax_rate, tax_rate_l);
  cout << "num:\t" << "income  " << "tax(before)  " << "tax(after)  " << '\n';
  if (n <= 100) {
    for (size_t i = 0; i < n; ++i) {
      cout << i + 1 << ":\t" <<
          regex_money(property[i], unit) << "    " <<
          regex_money(before[i], unit) << "    " <<
          regex_money(after[i], unit) << '\n';
    }
  }
  cout << "total:\t" << 
      regex_money(accumulate(property.begin(), property.end(), 0ull), unit) << "    " <<
      regex_money(accumulate(before.begin(), before.end(), 0ull), unit) << "    " <<
      regex_money(accumulate(after.begin(), after.end(), 0ull), unit) << '\n';
  return 0;
}
