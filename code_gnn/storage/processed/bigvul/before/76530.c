MAX_ROOM_NEEDED (const arguments *ap, size_t arg_index, FCHAR_T conversion,
                 arg_type type, int flags, size_t width, int has_precision,
                 size_t precision, int pad_ourselves)
{
  size_t tmp_length;

  switch (conversion)
    {
    case 'd': case 'i': case 'u':
# if HAVE_LONG_LONG_INT
      if (type == TYPE_LONGLONGINT || type == TYPE_ULONGLONGINT)
        tmp_length =
          (unsigned int) (sizeof (unsigned long long) * CHAR_BIT
                          * 0.30103  
                         )
          + 1;  
      else
# endif
      if (type == TYPE_LONGINT || type == TYPE_ULONGINT)
        tmp_length =
          (unsigned int) (sizeof (unsigned long) * CHAR_BIT
                          * 0.30103  
                         )
          + 1;  
      else
        tmp_length =
          (unsigned int) (sizeof (unsigned int) * CHAR_BIT
                          * 0.30103  
                         )
          + 1;  
      if (tmp_length < precision)
        tmp_length = precision;
       
      tmp_length = xsum (tmp_length, tmp_length);
       
      tmp_length = xsum (tmp_length, 1);
      break;

    case 'o':
# if HAVE_LONG_LONG_INT
      if (type == TYPE_LONGLONGINT || type == TYPE_ULONGLONGINT)
        tmp_length =
          (unsigned int) (sizeof (unsigned long long) * CHAR_BIT
                          * 0.333334  
                         )
          + 1;  
      else
# endif
      if (type == TYPE_LONGINT || type == TYPE_ULONGINT)
        tmp_length =
          (unsigned int) (sizeof (unsigned long) * CHAR_BIT
                          * 0.333334  
                         )
          + 1;  
      else
        tmp_length =
          (unsigned int) (sizeof (unsigned int) * CHAR_BIT
                          * 0.333334  
                         )
          + 1;  
      if (tmp_length < precision)
        tmp_length = precision;
       
      tmp_length = xsum (tmp_length, 1);
      break;

    case 'x': case 'X':
# if HAVE_LONG_LONG_INT
      if (type == TYPE_LONGLONGINT || type == TYPE_ULONGLONGINT)
        tmp_length =
          (unsigned int) (sizeof (unsigned long long) * CHAR_BIT
                          * 0.25  
                         )
          + 1;  
      else
# endif
      if (type == TYPE_LONGINT || type == TYPE_ULONGINT)
        tmp_length =
          (unsigned int) (sizeof (unsigned long) * CHAR_BIT
                          * 0.25  
                         )
          + 1;  
      else
        tmp_length =
          (unsigned int) (sizeof (unsigned int) * CHAR_BIT
                          * 0.25  
                         )
          + 1;  
      if (tmp_length < precision)
        tmp_length = precision;
       
      tmp_length = xsum (tmp_length, 2);
      break;

    case 'f': case 'F':
      if (type == TYPE_LONGDOUBLE)
        tmp_length =
          (unsigned int) (LDBL_MAX_EXP
                          * 0.30103  
                          * 2  
                         )
          + 1  
          + 10;  
      else
        tmp_length =
          (unsigned int) (DBL_MAX_EXP
                          * 0.30103  
                          * 2  
                         )
          + 1  
          + 10;  
      tmp_length = xsum (tmp_length, precision);
      break;

    case 'e': case 'E': case 'g': case 'G':
      tmp_length =
        12;  
      tmp_length = xsum (tmp_length, precision);
      break;

    case 'a': case 'A':
      if (type == TYPE_LONGDOUBLE)
        tmp_length =
          (unsigned int) (LDBL_DIG
                          * 0.831  
                         )
          + 1;  
      else
        tmp_length =
          (unsigned int) (DBL_DIG
                          * 0.831  
                         )
          + 1;  
      if (tmp_length < precision)
        tmp_length = precision;
       
      tmp_length = xsum (tmp_length, 12);
      break;

    case 'c':
# if HAVE_WINT_T && !WIDE_CHAR_VERSION
      if (type == TYPE_WIDE_CHAR)
        tmp_length = MB_CUR_MAX;
      else
# endif
        tmp_length = 1;
      break;

    case 's':
# if HAVE_WCHAR_T
      if (type == TYPE_WIDE_STRING)
        {
#  if WIDE_CHAR_VERSION
           
          const wchar_t *arg = ap->arg[arg_index].a.a_wide_string;

          if (has_precision)
            tmp_length = local_wcsnlen (arg, precision);
          else
            tmp_length = local_wcslen (arg);
#  else
           
           
          abort ();
#  endif
        }
      else
# endif
        {
# if WIDE_CHAR_VERSION
           
           
          abort ();
# else
           
          const char *arg = ap->arg[arg_index].a.a_string;

          if (has_precision)
            tmp_length = local_strnlen (arg, precision);
          else
            tmp_length = strlen (arg);
# endif
        }
      break;

    case 'p':
      tmp_length =
        (unsigned int) (sizeof (void *) * CHAR_BIT
                        * 0.25  
                       )
          + 1  
          + 2;  
      break;

    default:
      abort ();
    }

  if (!pad_ourselves)
    {
# if ENABLE_UNISTDIO
       
      tmp_length = xsum (tmp_length, width);
# else
       
      if (tmp_length < width)
        tmp_length = width;
# endif
    }

  tmp_length = xsum (tmp_length, 1);  

  return tmp_length;
}
