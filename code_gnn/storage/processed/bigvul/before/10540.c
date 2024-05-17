rotate_backups(const char *fname)
{
#ifdef __VMS
# define SEP "_"
# define AVS ";*"                        
# define AVSL (sizeof (AVS) - 1)
#else
# define SEP "."
# define AVSL 0
#endif

  int maxlen = strlen (fname) + sizeof (SEP) + numdigit (opt.backups) + AVSL;
  char *from = (char *)alloca (maxlen);
  char *to = (char *)alloca (maxlen);
  struct_stat sb;
  int i;

  if (stat (fname, &sb) == 0)
    if (S_ISREG (sb.st_mode) == 0)
      return;

  for (i = opt.backups; i > 1; i--)
    {
#ifdef VMS
       
      if (i == opt.backups)
        {
          sprintf (to, "%s%s%d%s", fname, SEP, i, AVS);
          delete (to);
        }
#endif
      sprintf (to, "%s%s%d", fname, SEP, i);
      sprintf (from, "%s%s%d", fname, SEP, i - 1);
      rename (from, to);
    }

  sprintf (to, "%s%s%d", fname, SEP, 1);
  rename(fname, to);
}
