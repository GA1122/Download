makedirs (char const *name)
{
  char *filename = xstrdup (name);
  char *f;
  char *flim = replace_slashes (filename);

  if (flim)
    {
       
      for (f = filename;  f <= flim;  f++)
	if (!*f)
	  {
	    mkdir (filename,
		   S_IRUSR|S_IWUSR|S_IXUSR
		   |S_IRGRP|S_IWGRP|S_IXGRP
		   |S_IROTH|S_IWOTH|S_IXOTH);
	    *f = '/';
	  }
    }
  free (filename);
}
