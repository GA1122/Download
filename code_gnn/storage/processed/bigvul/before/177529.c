static int merge_hist_buckets(struct hist_bucket *bucket,
 int max_buckets, int *num_buckets) {
 int small_bucket = 0, merge_bucket = INT_MAX, big_bucket = 0;
 int buckets = *num_buckets;
 int i;

  
  big_bucket = small_bucket = 0;
 for (i = 0; i < buckets; i++) {
 if (bucket[i].count < bucket[small_bucket].count)
      small_bucket = i;
 if (bucket[i].count > bucket[big_bucket].count)
      big_bucket = i;
 }

  
 while (buckets > max_buckets) {
 int last_bucket = buckets - 1;

  
 if (small_bucket == 0)
      merge_bucket = 1;
 else if (small_bucket == last_bucket)
      merge_bucket = last_bucket - 1;
 else if (bucket[small_bucket - 1].count < bucket[small_bucket + 1].count)
      merge_bucket = small_bucket - 1;
 else
      merge_bucket = small_bucket + 1;

    assert(abs(merge_bucket - small_bucket) <= 1);
    assert(small_bucket < buckets);
    assert(big_bucket < buckets);
    assert(merge_bucket < buckets);

 if (merge_bucket < small_bucket) {
      bucket[merge_bucket].high = bucket[small_bucket].high;
      bucket[merge_bucket].count += bucket[small_bucket].count;
 } else {
      bucket[small_bucket].high = bucket[merge_bucket].high;
      bucket[small_bucket].count += bucket[merge_bucket].count;
      merge_bucket = small_bucket;
 }

    assert(bucket[merge_bucket].low != bucket[merge_bucket].high);

    buckets--;

  
    big_bucket = small_bucket = 0;
 for (i = 0; i < buckets; i++) {
 if (i > merge_bucket)
        bucket[i] = bucket[i + 1];

 if (bucket[i].count < bucket[small_bucket].count)
        small_bucket = i;
 if (bucket[i].count > bucket[big_bucket].count)
        big_bucket = i;
 }
 }

 *num_buckets = buckets;
 return bucket[big_bucket].count;
}
