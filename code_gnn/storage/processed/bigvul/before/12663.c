find_var_ent(uschar * name)
{
int first = 0;
int last = var_table_size;

while (last > first)
  {
  int middle = (first + last)/2;
  int c = Ustrcmp(name, var_table[middle].name);

  if (c > 0) { first = middle + 1; continue; }
  if (c < 0) { last = middle; continue; }
  return &var_table[middle];
  }
return NULL;
}
