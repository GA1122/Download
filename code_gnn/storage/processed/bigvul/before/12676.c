restore_expand_strings(int save_expand_nmax, uschar **save_expand_nstring,
  int *save_expand_nlength)
{
int i;
expand_nmax = save_expand_nmax;
for (i = 0; i <= expand_nmax; i++)
  {
  expand_nstring[i] = save_expand_nstring[i];
  expand_nlength[i] = save_expand_nlength[i];
  }
}
