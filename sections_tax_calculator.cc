#include <algorithm>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>
#include <random>
using namespace std;
vector<long long> const kInc = {
  1, 10, 100, 1000, 10000,
  100000, 1000000, 10000000, 100000000, 1000000000,
  10000000000, 100000000000, 1000000000000, 10000000000000, 100000000000000,
};
vector<double> const kTax = {
  0.0, 0.0, 0.0, 0.0, 0.0,
  1.0, 2.0, 4.0, 12.0, 20.0,
  24.0, 15.0, 10.0, 7.0, 5.0,
};
int argument_check(int ac, char **av, long long &n, long long &sect) {
  if (ac != 2 && ac != 3) {
    cerr << "invaild argument:" << '\n';
    cerr << "  usage:" << '\n';
    cerr << "    ./[a.exe] <n> [section]" << '\n';
    cerr << "    <n>: Number of social members." << '\n';
    cerr << "    [section]: Number of income sections." << '\n';
    return -1;
  }
  n = atoll(av[1]);
  sect = kInc.size() - 1;
  if (ac == 3) { sect = atoll(av[2]); }
  if (n < (long long)1e0 || n > (long long)1e8) {
    cerr << "invaild argument:" << '\n';
    cerr << "  n:" << '\n';
    cerr << "    1e0 <= n && n <= 1e8" << '\n';
    return -2;
  }
  if (sect < (long long)1e0 || sect >= (long long)kInc.size()) {
    cerr << "invaild argument:" << '\n';
    cerr << "  section:" << '\n';
    cerr << "    1e0 <= section && section < " << kInc.size() << '\n';
    return -3;
  }
  if (accumulate(kTax.begin(), kTax.end(), 0.0) != 100.0) {
    cerr << "invaild argument:" << '\n';
    cerr << "  kTax:" << '\n';
    cerr << "    accumulate(kTax) == 100.0" << '\n';
    return -4;
  }
  if (kInc.size() != kTax.size()) {
    cerr << "invaild argument:" << '\n';
    cerr << "  kInc, kTax:" << '\n';
    cerr << "    kInc.size() == kTax.size()" << '\n';
    return -5;
  }
  return 0;
}
template<class T> T __random(T mnm, T mxm) {
  static mt19937 gen((random_device())());
  return (uniform_int_distribution<T>(mnm, mxm))(gen);
}
void generate_random(vector<long long> &income, long long n, long long sect) {
  long long div = n / sect;
  long long rmn = n % sect;
  long long val = 1;
  size_t ix = 0;
  income.resize(n);
  for (size_t i = 0; i < sect; ++i) {
    for (size_t j = 0; j < div + (rmn > i); ++j) {
      income[ix++] = __random(val, val * 10);
    }
    val *= 10;
  }
}
void taxrate_calculate_before(vector<long long> const &income,
                              vector<long long> &before) {
  size_t n = income.size();
  before.resize(n);
  for (size_t i = 0; i < n; ++i) {
    if (income[i] <= 12'000'000) {
      before[i] = income[i] * 0.06;
    } else if (income[i] <= 46'000'000) {
      before[i] = income[i] * 0.15 - 1'080'000;
    } else if (income[i] <= 88'000'000) {
      before[i] = income[i] * 0.24 - 5'220'000;
    } else if (income[i] <= 150'000'000) {
      before[i] = income[i] * 0.35 - 14'900'000;
    } else if (income[i] <= 300'000'000) {
      before[i] = income[i] * 0.38 - 19'400'000;
    } else if (income[i] <= 500'000'000) {
      before[i] = income[i] * 0.40 - 25'400'000;
    } else if (income[i] <= 1'000'000'000) {
      before[i] = income[i] * 0.42 - 35'400'000;
    } else {
      before[i] = income[i] * 0.45 - 65'400'000;
    }
  }
}
void taxrate_calculate_after(vector<long long> const &income,
                             vector<long long> &after,
                             vector<double> &taxrates) {
  size_t n = income.size();
  after.resize(n);
  taxrates.resize(n);
  for (size_t i = 0; i < n; ++i) {
    for (size_t j = 1; j < kInc.size(); ++j) {
      if (income[i] < kInc[j]) {
        double ratio = (double)max(0ll, income[i] - kInc[j - 1]) /
                       (double)(kInc[j] - kInc[j - 1]);
        taxrates[i] += ratio * kTax[j];
        break;
      }
      taxrates[i] += kTax[j];
    }
    after[i] = (taxrates[i] / 1e2) * income[i];
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
  vector<long long> income, before, after;
  vector<double> taxrates;
  long long n, sect;
  double taxrate;
  size_t unit = 3;
  if (int tmp = argument_check(ac, av, n, sect)) {
    return tmp;
  }
  generate_random(income, n, sect);
  taxrate_calculate_before(income, before);
  taxrate_calculate_after(income, after, taxrates);
  cout << fixed << setprecision(2);
  cout << "Num:\t" << "income  " << "tax(before)  " << "tax(after)  " <<
      "taxrate(after)  " << "pure_income(after)" << '\n';
  for (size_t i = 0; i < n; ++i) {
    cout << i + 1 << ":\t" <<
        regex_money(income[i], unit) << "    " <<
        regex_money(before[i], unit) << "    " <<
        regex_money(after[i], unit) << "    " <<
        taxrates[i] << "%    " <<
        regex_money(income[i] - after[i], unit) << '\n';
  }
  cout << "total:\t" << 
      regex_money(accumulate(income.begin(), income.end(), 0ull), unit) << "    " <<
      regex_money(accumulate(before.begin(), before.end(), 0ull), unit) << "    " <<
      regex_money(accumulate(after.begin(), after.end(), 0ull), unit) << '\n';
  return 0;
}
