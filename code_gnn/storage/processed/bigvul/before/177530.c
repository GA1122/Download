static void show_histogram(const struct hist_bucket *bucket,
 int buckets, int total, int scale) {
 const char *pat1, *pat2;
 int i;

 switch ((int)(log(bucket[buckets - 1].high) / log(10)) + 1) {
 case 1:
 case 2:
      pat1 = "%4d %2s: ";
      pat2 = "%4d-%2d: ";
 break;
 case 3:
      pat1 = "%5d %3s: ";
      pat2 = "%5d-%3d: ";
 break;
 case 4:
      pat1 = "%6d %4s: ";
      pat2 = "%6d-%4d: ";
 break;
 case 5:
      pat1 = "%7d %5s: ";
      pat2 = "%7d-%5d: ";
 break;
 case 6:
      pat1 = "%8d %6s: ";
      pat2 = "%8d-%6d: ";
 break;
 case 7:
      pat1 = "%9d %7s: ";
      pat2 = "%9d-%7d: ";
 break;
 default:
      pat1 = "%12d %10s: ";
      pat2 = "%12d-%10d: ";
 break;
 }

 for (i = 0; i < buckets; i++) {
 int len;
 int j;
 float pct;

    pct = (float)(100.0 * bucket[i].count / total);
    len = HIST_BAR_MAX * bucket[i].count / scale;
 if (len < 1)
      len = 1;
    assert(len <= HIST_BAR_MAX);

 if (bucket[i].low == bucket[i].high)
      fprintf(stderr, pat1, bucket[i].low, "");
 else
      fprintf(stderr, pat2, bucket[i].low, bucket[i].high);

 for (j = 0; j < HIST_BAR_MAX; j++)
      fprintf(stderr, j < len ? "=" : " ");
    fprintf(stderr, "\t%5d (%6.2f%%)\n", bucket[i].count, pct);
 }
}
