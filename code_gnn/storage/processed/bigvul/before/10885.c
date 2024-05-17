append_cert_policy (char **policies, const char *oid, int crit)
{
  char *p;

  if (!*policies)
    {
      *policies = xtrymalloc (strlen (oid) + 4);
      if (!*policies)
        return gpg_error (GPG_ERR_ENOMEM);
      p = *policies;
    }
  else
    {
      char *tmp = xtryrealloc (*policies,
                               strlen(*policies) + 1 + strlen (oid) + 4);
      if (!tmp)
        return gpg_error (GPG_ERR_ENOMEM);
      *policies = tmp;
      p = stpcpy (tmp+strlen (tmp), "\n");;
    }

  strcpy (stpcpy (p, oid), crit? ":C:": ":N:");
  return 0;
}
