uint32_t get_int(char **p, int DefaultValue)
{
 uint32_t Value = 0;
 unsigned char UseDefault;

 UseDefault = 1;
  skip_blanks(p);

 while ( ((**p)<= '9' && (**p)>= '0') )
 {
 Value = Value * 10 + (**p) - '0';
 UseDefault = 0;
 (*p)++;
 }

 if (UseDefault)
 return DefaultValue;
 else
 return Value;
}
