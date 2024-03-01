#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <random>
using namespace std;
int argument_check(int ac,
                   char **av,
                   long long &n,
                   long long &mn,
                   long long &mx,
                   long long &benchmark,
                   double &tax_rate) {
  if (ac != 6) {
    cerr << "invaild argument:" << '\n';
    cerr << "  usage:" << '\n';
    cerr << "    ./[a.exe] <n> <min> <max> <tax_rate> <benchmark>" << '\n';
    cerr << "    <n>: Number of social members." << '\n';
    cerr << "    <min>: Minimum income." << '\n';
    cerr << "    <max>: Maximum income." << '\n';
    cerr << "    <benchmark>: Benchmark max income." << '\n';
    cerr << "    <tax_rate>: Max tax rate in next system." << '\n';
    return -1;
  }
  n = atoll(av[1]);
  mn = atoll(av[2]);
  mx = atoll(av[3]);
  benchmark = atoll(av[4]);
  tax_rate = atof(av[5]);
  if (n < (long long)1e0 || n > (long long)1e8) {
    cerr << "invaild argument:" << '\n';
    cerr << "  n:" << '\n';
    cerr << "    1e0 <= n && n <= 1e8" << '\n';
    return -2;
  }
  if (mn < (long long)1e0 || mn > (long long)1e12) {
    cerr << "invaild argument:" << '\n';
    cerr << "  min:" << '\n';
    cerr << "    1e0 <= min && min <= 1e12" << '\n';
    return -3;
  }
  if (mx < (long long)1e0 || mx > (long long)1e12) {
    cerr << "invaild argument:" << '\n';
    cerr << "  max:" << '\n';
    cerr << "    1e0 <= max && max <= 1e12" << '\n';
    return -4;
  }
  if (mn > mx) {
    cerr << "invaild argument:" << '\n';
    cerr << "  min & max:" << '\n';
    cerr << "    min <= max" << '\n';
    return -5;
  }
  if (benchmark < (long long)1e0 || benchmark > (long long)1e12) {
    cerr << "invaild argument:" << '\n';
    cerr << "  benchmark:" << '\n';
    cerr << "    1e0 <= benchmark && benchmark <= 1e12" << '\n';
    return -6;
  }
  if (tax_rate < 1e-5 || tax_rate > 1e5) {
    cerr << "invaild argument:" << '\n';
    cerr << "  tax_rate:" << '\n';
    cerr << "    1e-5 <= tax_rate && tax_rate <= 1e5" << '\n';
    return -7;
  }
  return 0;
}
template<class T> T __random(T mnm, T mxm) {
  static mt19937 gen((random_device())());
  return (uniform_int_distribution<T>(mnm, mxm))(gen);
}
void random_generate(vector<long long> &property,
                     long long n,
                     long long mn,
                     long long mx) {
  property.resize(n);
  for (size_t i = 0; i < n; ++i) {
    property[i] = __random(mn, mx);
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
                              long long benchmark,
                              double tax_rate) {
  size_t n = property.size();
  after.resize(n);
  for (size_t i = 0; i < n; ++i) {
    after[i] = (tax_rate / 1e2) *
        ((double)property[i] / (double)benchmark) * property[i];
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
void print_vector(vector<long long> const &v, size_t unit) {
  for (size_t i = 0; i < v.size(); ++i) {
    cout << regex_money(v[i], unit) << " \n"[i == v.size() - 1];
  }
}
int main(int ac, char **av) {
  ios::sync_with_stdio(0), cin.tie(0), cout.tie(0);
  vector<long long> property, before, after;
  long long n, mn, mx, benchmark;
  double tax_rate;
  size_t unit = 3;
  if (int tmp = argument_check(ac, av, n, mn, mx, benchmark, tax_rate)) {
    return tmp;
  }
  random_generate(property, n, mn, mx);
  tax_rate_calculate_before(property, before);
  tax_rate_calculate_after(property, after, benchmark, tax_rate);
  // print_vector(property, unit);
  // print_vector(before, unit);
  // print_vector(after, unit);
  cout << regex_money(
      accumulate(property.begin(), property.end(), 0ull), unit) << '\n';
  cout << regex_money(
      accumulate(before.begin(), before.end(), 0ull), unit) << '\n';
  cout << regex_money(
      accumulate(after.begin(), after.end(), 0ull), unit) << '\n';
  return 0;
}
