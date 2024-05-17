save_expand_strings(uschar **save_expand_nstring, int *save_expand_nlength)
{
int i;
for (i = 0; i <= expand_nmax; i++)
  {
  save_expand_nstring[i] = expand_nstring[i];
  save_expand_nlength[i] = expand_nlength[i];
  }
return expand_nmax;
}
