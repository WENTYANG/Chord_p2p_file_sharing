#include "util.h"

#include <assert.h>
#include <time.h>

#include <cmath>
#include <cstdlib>

#include "util/config.h"

using namespace std;

/*
  Return a random port number from 10000 to 65535.
*/
int get_random_port() {
  //srand((unsigned)time(NULL));
  int port = (rand() % (65535 - 10000 + 1)) + 10000;
  return port;
}

/*
  Calculate the distance from node base to node target.
  Distance is defined as the number of steps that needs for a point
  moving from base to targe in wise-clock direction in the circle.
  @param base: the index of node base
  @param target: the index of node target
  @param N: the number of bits to represent the values in the circle
  @return return the distance
*/
digest_t get_distance(digest_t base, digest_t target, digest_t N) {
  digest_t d = target - base;
  if (d < 0) {
    d += pow(2, N);
  } else if(d == 0){
    d = pow(2, N); // distance equal to a whole circle
  }
  return d;
}

/*
  Implement add operation for hash value in the chord circle.
  @return the result of hash + num.
*/
digest_t hash_value_add(digest_t hash, digest_t num) {
  assert(abs(num) < my_config::total_hash_points);

  digest_t res = hash + num;
  if (res < 0) {
    res += my_config::total_hash_points;
  } else if (res >= my_config::total_hash_points) {
    res -= my_config::total_hash_points;
  }
  return res;
}

/*
 print out contactInfo_t
*/
void print_contactInfo(contactInfo_t t) {
  cout << "hostname: " << t.first << " port: " << t.second << endl;
}


void print_DHT(unordered_map<digest_t, contactInfo_t> DHT) {
  for (auto iter : DHT) {
    cout << "digest: " << iter.first << " contactInfo_t: " << iter.second.first << " : " << iter.second.second << endl;
  }
}
