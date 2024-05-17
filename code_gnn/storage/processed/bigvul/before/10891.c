ksba_name_release (ksba_name_t name)
{
  int i;

  if (!name)
    return;
  if (name->ref_count < 1)
    {
      fprintf (stderr, "BUG: trying to release an already released name\n");
      return;
    }
  if (--name->ref_count)
    return;

  for (i=0; i < name->n_names; i++)
    xfree (name->names[i]);
  xfree (name->names);
  name->n_names = 0;
  xfree (name);
}
