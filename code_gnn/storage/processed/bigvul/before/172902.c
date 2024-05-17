uint32_t get_hex(char **p, int DefaultValue)
{
 uint32_t Value = 0;
 unsigned char UseDefault;

 UseDefault = 1;
  skip_blanks(p);

 while ( ((**p)<= '9' && (**p)>= '0') ||
 ((**p)<= 'f' && (**p)>= 'a') ||
 ((**p)<= 'F' && (**p)>= 'A') )
 {
 if (**p >= 'a')
 Value = Value * 16 + (**p) - 'a' + 10;
 else if (**p >= 'A')
 Value = Value * 16 + (**p) - 'A' + 10;
 else
 Value = Value * 16 + (**p) - '0';
 UseDefault = 0;
 (*p)++;
 }

 if (UseDefault)
 return DefaultValue;
 else
 return Value;
}
