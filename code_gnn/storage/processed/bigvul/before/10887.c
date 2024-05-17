ksba_name_enum (ksba_name_t name, int idx)
{
  if (!name || idx < 0)
    return NULL;
  if (idx >= name->n_names)
    return NULL;   

  return name->names[idx];
}
