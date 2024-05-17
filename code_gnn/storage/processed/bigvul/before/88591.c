static void load_creator_from_buf(FILE *fp, xref_t *xref, const char *buf)
{
    int   is_xml;
    char *c;

    if (!buf)
      return;

     
    if ((c = strstr(buf, "/Type")))
      while (*c && !isspace(*c))
        ++c;

     
    is_xml = 0;
    if (c && (*c == 'M'))
      is_xml = 1;

     
    if (is_xml)
      load_creator_from_xml(xref, buf);
    else
      load_creator_from_old_format(fp, xref, buf);
}
