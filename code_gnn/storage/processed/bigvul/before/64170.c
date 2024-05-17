iterate (const char *filename,
	 enum grub_fshelp_filetype filetype,
	 grub_fshelp_node_t node,
	 void *closure)
{
  struct find_file_closure *c = closure;

  if (filetype == GRUB_FSHELP_UNKNOWN ||
      (grub_strcmp (c->name, filename) &&
       (! (filetype & GRUB_FSHELP_CASE_INSENSITIVE) ||
	grub_strncasecmp (c->name, filename, GRUB_LONG_MAX))))
    {
      grub_free (node);
      return 0;
    }

   
  *(c->type) = filetype & ~GRUB_FSHELP_CASE_INSENSITIVE;
  *(c->oldnode) = *(c->currnode);
  *(c->currnode) = node;

  return 1;
}