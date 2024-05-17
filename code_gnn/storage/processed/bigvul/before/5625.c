open_patch_file (char const *filename)
{
    file_offset file_pos = 0;
    struct stat st;
    if (!filename || !*filename || strEQ (filename, "-"))
      {
	file_offset stdin_pos;
#if HAVE_SETMODE_DOS
	if (binary_transput)
	  {
	    if (isatty (STDIN_FILENO))
	      fatal ("cannot read binary data from tty on this platform");
	    setmode (STDIN_FILENO, O_BINARY);
	  }
#endif
	if (fstat (STDIN_FILENO, &st) != 0)
	  pfatal ("fstat");
	if (S_ISREG (st.st_mode) && (stdin_pos = file_tell (stdin)) != -1)
	  {
	    pfp = stdin;
	    file_pos = stdin_pos;
	  }
	else
	  {
	    size_t charsread;
	    int fd = make_tempfile (&TMPPATNAME, 'p', NULL, O_RDWR | O_BINARY, 0);
	    TMPPATNAME_needs_removal = 1;
	    pfp = fdopen (fd, "w+b");
	    if (! pfp)
	      pfatal ("Can't open stream for file %s", quotearg (TMPPATNAME));
	    for (st.st_size = 0;
		 (charsread = fread (buf, 1, bufsize, stdin)) != 0;
		 st.st_size += charsread)
	      if (fwrite (buf, 1, charsread, pfp) != charsread)
		write_fatal ();
	    if (ferror (stdin) || fclose (stdin) != 0)
	      read_fatal ();
	    if (fflush (pfp) != 0
		|| file_seek (pfp, (file_offset) 0, SEEK_SET) != 0)
	      write_fatal ();
	  }
      }
    else
      {
	pfp = fopen (filename, binary_transput ? "rb" : "r");
	if (!pfp)
	  pfatal ("Can't open patch file %s", quotearg (filename));
	if (fstat (fileno (pfp), &st) != 0)
	  pfatal ("fstat");
      }
    p_filesize = st.st_size;
    if (p_filesize != (file_offset) p_filesize)
      fatal ("patch file is too long");
    next_intuit_at (file_pos, 1);
    set_hunkmax();
}
