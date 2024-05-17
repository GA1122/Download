ksba_name_new (ksba_name_t *r_name)
{
  *r_name = xtrycalloc (1, sizeof **r_name);
  if (!*r_name)
    return gpg_error_from_errno (errno);
  (*r_name)->ref_count++;

  return 0;
}
