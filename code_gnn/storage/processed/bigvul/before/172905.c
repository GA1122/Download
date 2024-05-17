void get_str(char **p, char *Buffer)
{
  skip_blanks(p);

 while (**p != 0 && **p != ' ')
 {
 *Buffer = **p;
 (*p)++;
 Buffer++;
 }

 *Buffer = 0;
}
