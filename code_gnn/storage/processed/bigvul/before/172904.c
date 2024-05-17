int get_signed_int(char **p, int DefaultValue)
{
 int Value = 0;
 unsigned char UseDefault;
 unsigned char NegativeNum = 0;

 UseDefault = 1;
  skip_blanks(p);

 if ( (**p) == '-')
 {
 NegativeNum = 1;
 (*p)++;
 }
 while ( ((**p)<= '9' && (**p)>= '0') )
 {
 Value = Value * 10 + (**p) - '0';
 UseDefault = 0;
 (*p)++;
 }

 if (UseDefault)
 return DefaultValue;
 else
 return ((NegativeNum == 0)? Value : -Value);
}
