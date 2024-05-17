long long stringToIntWithRadix(const char *s,
               int forceRadix,  
               bool *hasError,  
               const char **endOfInteger  
               ) {
  while (isWhitespace(*s)) s++;

  bool isNegated = false;
  long long v = 0;
  if (*s == '-') {
    isNegated = true;
    s++;
  } else if (*s == '+') {
    s++;
  }

  const char *numberStart = s;
  if (endOfInteger) (*endOfInteger)=s;

  int radix = getRadix(&s, forceRadix, hasError);
  if (!radix) return 0;

  while (*s) {
    int digit = chtod(*s);
    if (digit<0 || digit>=radix)
      break;
    v = v*radix + digit;
    s++;
  }

  if (hasError)
    *hasError = s==numberStart;  
  if (endOfInteger) (*endOfInteger)=s;

  if (isNegated) return -v;
  return v;
}
