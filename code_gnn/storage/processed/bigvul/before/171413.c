static unsigned unhex(unsigned char* s, int len) {
 unsigned n = 0, c;

 while (len-- > 0) {
 switch ((c = *s++)) {
 case '0':
 case '1':
 case '2':
 case '3':
 case '4':
 case '5':
 case '6':
 case '7':
 case '8':
 case '9':
                c -= '0';
 break;
 case 'a':
 case 'b':
 case 'c':
 case 'd':
 case 'e':
 case 'f':
                c = c - 'a' + 10;
 break;
 case 'A':
 case 'B':
 case 'C':
 case 'D':
 case 'E':
 case 'F':
                c = c - 'A' + 10;
 break;
 default:
 return 0xffffffff;
 }

        n = (n << 4) | c;
 }

 return n;
}
