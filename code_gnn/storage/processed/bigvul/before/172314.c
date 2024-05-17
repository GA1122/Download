static byte parseHexChar(char ch) {
 if (isdigit(ch))
 return ch - '0';
 else if ('A' <= ch && ch <= 'F')
 return ch - 'A' + 10;
 else if ('a' <= ch && ch <= 'f')
 return ch - 'a' + 10;
 else {
        ALOGE("invalid character in bssid %c", ch);
 return 0;
 }

 }
