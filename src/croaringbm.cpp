#include <chrono>
#include <iostream>
#include <vector>

#include "roaring.hh"
#include "roaring.c"

using namespace std;
using namespace std::chrono;

void merge(vector<Roaring>& bitmaps) {
  milliseconds mergeStart = duration_cast< milliseconds >(
    system_clock::now().time_since_epoch()
  );

  Roaring merged;
  for (auto it = bitmaps.begin(); it != bitmaps.end(); it++) {
    merged |= *it;
  }

  milliseconds mergeEnd = duration_cast< milliseconds >(
    system_clock::now().time_since_epoch()
  );

  cout << "Merged in " << mergeEnd.count() - mergeStart.count() << "ms" << endl;

  cout << "cardinality = " << merged.cardinality() << endl;
  cout << "portable size = " << merged.getSizeInBytes() << endl;
}

void mergeFast(vector<Roaring>& bitmaps) {
  milliseconds mergeStart = duration_cast< milliseconds >(
    system_clock::now().time_since_epoch()
  );

  vector<Roaring const*> bitmap_ptrs;
  bitmap_ptrs.reserve(bitmaps.size());

  for (auto const & x : bitmaps) {
    bitmap_ptrs.push_back(&x);
  }

  Roaring merged;
  merged.fastunion(bitmap_ptrs.size(), bitmap_ptrs.data());

  milliseconds mergeEnd = duration_cast< milliseconds >(
    system_clock::now().time_since_epoch()
  );

  cout << "Batches merged in " << mergeEnd.count() - mergeStart.count() << "ms" << endl;

  cout << "cardinality = " << merged.cardinality() << endl;
  cout << "portable size = " << merged.getSizeInBytes() << endl;
}

int main() {
    milliseconds buildStart = duration_cast< milliseconds >(
    system_clock::now().time_since_epoch()
  );

  uint32_t counter = 0;
  vector<Roaring> bitmaps;

  for (uint32_t i = 0; i < 10000; i++) {
    Roaring bitmap;
    for (uint32_t j = 0; j < 25000; j++) {
      bitmap.add(counter++);
    }
    bitmaps.push_back(bitmap);
  }

  milliseconds buildEnd = duration_cast< milliseconds >(
    system_clock::now().time_since_epoch()
  );

  cout << "Built in " << buildEnd.count() - buildStart.count() << "ms" << endl;

  merge(bitmaps);
  mergeFast(bitmaps);
  return 0;
}
