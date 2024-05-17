is_invalid_entry (struct fileinfo *f)
{
  struct fileinfo *cur = f;
  char *f_name = f->name;

   
  while (cur->next)
    {
      cur = cur->next;
      if (strcmp(f_name, cur->name) == 0)
          return true;
    }
  return false;
}
