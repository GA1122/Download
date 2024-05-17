request_set_user_header (struct request *req, const char *header)
{
  char *name;
  const char *p = strchr (header, ':');
  if (!p)
    return;
  BOUNDED_TO_ALLOCA (header, p, name);
  ++p;
  while (c_isspace (*p))
    ++p;
  request_set_header (req, xstrdup (name), (char *) p, rel_name);
}
