get_file_flags (const char *filename, int *dt)
{
  logprintf (LOG_VERBOSE, _("\
File %s already there; not retrieving.\n\n"), quote (filename));
   
  *dt |= RETROKF;

   
   
  if (has_html_suffix_p (filename))
    *dt |= TEXTHTML;
}
