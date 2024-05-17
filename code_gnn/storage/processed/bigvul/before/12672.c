read_name(uschar *name, int max, uschar *s, uschar *extras)
{
int ptr = 0;
while (*s != 0 && (isalnum(*s) || Ustrchr(extras, *s) != NULL))
  {
  if (ptr < max-1) name[ptr++] = *s;
  s++;
  }
name[ptr] = 0;
return s;
}
