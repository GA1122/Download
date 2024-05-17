callout_name_table_hash(st_callout_name_key* x)
{
  UChar *p;
  int val = 0;

  p = x->s;
  while (p < x->end) {
    val = val * 997 + (int )*p++;
  }

   
  return val + (val >> 5) + ((intptr_t )x->enc & 0xffff) + x->type;
}
