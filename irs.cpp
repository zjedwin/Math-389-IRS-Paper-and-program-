#include <vector>
#include <iostream>
#include <iomanip>
#include <cassert>
#include <algorithm>

using namespace std;

double getWallTime(){
  return static_cast<double>(clock()) / CLOCKS_PER_SEC;
}

void claimNumber(unsigned n, vector<bool> &claimed, unsigned long long &score){
  assert(n < claimed.size());
	
  claimed[n]  = true;
  score      += n;
  // all proper divisors of n are claimed by the IRS
  for(unsigned ix = 1; ix <= n/2; ++ix)
  {
    if((n % ix == 0) && !claimed[ix])
      claimed[ix] = true;
  }
}

bool hasDivisor(const unsigned n, const vector<bool> &claimed, int lb)
{
	lb = max(lb,1);
  for( ; lb <= n/2; ++lb)
	{
    if((n % lb == 0) && !claimed[lb])
			return true;
	}
  return false;
}

void simGame(unsigned N)
{
  static const vector<int> primes = {2,3,5,7};
  // The claimed vector has N+1 entries so that
  // to check that an element k is claimed, we
  // look at claimed[k] instead of claimed[k-1]
  vector<bool> claimed(N+1, false);
  unsigned long long playerScore = 0;
	
  for(int ix = primes.size()-1; ix >= 0; --ix)
  {
    int p      = primes[ix];
    unsigned n = N/2 + 1;

    // If n is not a multiple of p, set n to be
    // the nearest larger mutliple of p.
    if(n%p != 0)
    {
      n += p - (n%p);
      if(n >= claimed.size())
        continue;
    }
    // sanity check
    assert(n%p == 0);

    for( ; n <= N; n += p)
    {
      bool unseen = true;
      // Check that this number is not divisible
      // by a prime we have already looked at
      for(int jx = 0; jx < ix; ++jx)
      {
        if(n % primes[jx] == 0){
          unseen = false;
          break;
        }
      }
			
      if(unseen && hasDivisor(n, claimed, (N+1)/(2*p)))
        claimNumber(n, claimed, playerScore);
    }
  }

  auto totalScore    = (N * (N+1))/2;
  auto computerScore = totalScore - playerScore;
	
  cout << "N: "                << N
       << "\tRatio: "          << setprecision(4) << (long double) playerScore/totalScore
       << "\tPlayer Score: "   << playerScore
       << "\tComputer Score: " << computerScore   << endl;
}

int main(int argc, char *argv[])
{
  double startTime = getWallTime();
  int N = 1;
	
  if(argc == 2) N = atoi(argv[1]);
  if(N >= 1) simGame(N);
  /*
  for(int ix = 1; ix <= N; ++ix)
    simGame(ix);
  */

  double totalTime = getWallTime() - startTime;
  cout << "\nTime elapsed : " << totalTime << " sec\n";
  return 0;
}
