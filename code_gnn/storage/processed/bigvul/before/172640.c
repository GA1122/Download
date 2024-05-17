static void float2urat(double value, unsigned int max, unsigned int *numerator,
 unsigned int *denominator) {
 if (value <= 0) {
 *numerator = 0;
 *denominator = 1;
 return;
 }

 if (value > max) {
 *numerator = max;
 *denominator = 1;
 return;
 }

 if (value < 1e-9) {
 unsigned int n = (unsigned int)(value * max);
 if (n == 0) {
 *numerator = 0;
 *denominator = 1;
 } else {
 *numerator = n;
 *denominator = max;
 }
 return;
 }

 unsigned int d;
 for (d = 1000000000; d >= 1; d /= 10) {
 double s = value * d;
 if (s <= max) {
 unsigned int n = (unsigned int)s;
 while (n % 10 == 0 && d >= 10) {
                n /= 10;
                d /= 10;
 }
 *numerator = n;
 *denominator = d;
 return;
 }
 }

 *numerator = 0;
 *denominator = 1;
}
