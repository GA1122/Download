chop_match(uschar *name, uschar **table, int table_size)
{
uschar **bot = table;
uschar **top = table + table_size;

while (top > bot)
  {
  uschar **mid = bot + (top - bot)/2;
  int c = Ustrcmp(name, *mid);
  if (c == 0) return mid - table;
  if (c > 0) bot = mid + 1; else top = mid;
  }

return -1;
}
