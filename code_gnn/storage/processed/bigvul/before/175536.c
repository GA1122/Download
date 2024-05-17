static char encode6Bit(unsigned x) {
 if (x <= 25) {
 return 'A' + x;
 } else if (x <= 51) {
 return 'a' + x - 26;
 } else if (x <= 61) {
 return '0' + x - 52;
 } else if (x == 62) {
 return '+';
 } else {
 return '/';
 }
}
