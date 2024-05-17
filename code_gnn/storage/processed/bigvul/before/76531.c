VASNPRINTF (DCHAR_T *resultbuf, size_t *lengthp,
            const FCHAR_T *format, va_list args)
{
  DIRECTIVES d;
  arguments a;

  if (PRINTF_PARSE (format, &d, &a) < 0)
     
    return NULL;

#define CLEANUP() \
  if (d.dir != d.direct_alloc_dir)                                      \
    free (d.dir);                                                       \
  if (a.arg != a.direct_alloc_arg)                                      \
    free (a.arg);

  if (PRINTF_FETCHARGS (args, &a) < 0)
    {
      CLEANUP ();
      errno = EINVAL;
      return NULL;
    }

  {
    size_t buf_neededlength;
    TCHAR_T *buf;
    TCHAR_T *buf_malloced;
    const FCHAR_T *cp;
    size_t i;
    DIRECTIVE *dp;
     
    DCHAR_T *result;
    size_t allocated;
    size_t length;

     
    buf_neededlength =
      xsum4 (7, d.max_width_length, d.max_precision_length, 6);
#if HAVE_ALLOCA
    if (buf_neededlength < 4000 / sizeof (TCHAR_T))
      {
        buf = (TCHAR_T *) alloca (buf_neededlength * sizeof (TCHAR_T));
        buf_malloced = NULL;
      }
    else
#endif
      {
        size_t buf_memsize = xtimes (buf_neededlength, sizeof (TCHAR_T));
        if (size_overflow_p (buf_memsize))
          goto out_of_memory_1;
        buf = (TCHAR_T *) malloc (buf_memsize);
        if (buf == NULL)
          goto out_of_memory_1;
        buf_malloced = buf;
      }

    if (resultbuf != NULL)
      {
        result = resultbuf;
        allocated = *lengthp;
      }
    else
      {
        result = NULL;
        allocated = 0;
      }
    length = 0;
     

     
#define ENSURE_ALLOCATION(needed) \
    if ((needed) > allocated)                                                \
      {                                                                      \
        size_t memory_size;                                                  \
        DCHAR_T *memory;                                                     \
                                                                             \
        allocated = (allocated > 0 ? xtimes (allocated, 2) : 12);            \
        if ((needed) > allocated)                                            \
          allocated = (needed);                                              \
        memory_size = xtimes (allocated, sizeof (DCHAR_T));                  \
        if (size_overflow_p (memory_size))                                   \
          goto out_of_memory;                                                \
        if (result == resultbuf || result == NULL)                           \
          memory = (DCHAR_T *) malloc (memory_size);                         \
        else                                                                 \
          memory = (DCHAR_T *) realloc (result, memory_size);                \
        if (memory == NULL)                                                  \
          goto out_of_memory;                                                \
        if (result == resultbuf && length > 0)                               \
          DCHAR_CPY (memory, result, length);                                \
        result = memory;                                                     \
      }

    for (cp = format, i = 0, dp = &d.dir[0]; ; cp = dp->dir_end, i++, dp++)
      {
        if (cp != dp->dir_start)
          {
            size_t n = dp->dir_start - cp;
            size_t augmented_length = xsum (length, n);

            ENSURE_ALLOCATION (augmented_length);
             
            if (sizeof (FCHAR_T) == sizeof (DCHAR_T))
              {
                DCHAR_CPY (result + length, (const DCHAR_T *) cp, n);
                length = augmented_length;
              }
            else
              {
                do
                  result[length++] = *cp++;
                while (--n > 0);
              }
          }
        if (i == d.count)
          break;

         
        if (dp->conversion == '%')
          {
            size_t augmented_length;

            if (!(dp->arg_index == ARG_NONE))
              abort ();
            augmented_length = xsum (length, 1);
            ENSURE_ALLOCATION (augmented_length);
            result[length] = '%';
            length = augmented_length;
          }
        else
          {
            if (!(dp->arg_index != ARG_NONE))
              abort ();

            if (dp->conversion == 'n')
              {
                switch (a.arg[dp->arg_index].type)
                  {
                  case TYPE_COUNT_SCHAR_POINTER:
                    *a.arg[dp->arg_index].a.a_count_schar_pointer = length;
                    break;
                  case TYPE_COUNT_SHORT_POINTER:
                    *a.arg[dp->arg_index].a.a_count_short_pointer = length;
                    break;
                  case TYPE_COUNT_INT_POINTER:
                    *a.arg[dp->arg_index].a.a_count_int_pointer = length;
                    break;
                  case TYPE_COUNT_LONGINT_POINTER:
                    *a.arg[dp->arg_index].a.a_count_longint_pointer = length;
                    break;
#if HAVE_LONG_LONG_INT
                  case TYPE_COUNT_LONGLONGINT_POINTER:
                    *a.arg[dp->arg_index].a.a_count_longlongint_pointer = length;
                    break;
#endif
                  default:
                    abort ();
                  }
              }
#if ENABLE_UNISTDIO
             
            else if (dp->conversion == 'U')
              {
                arg_type type = a.arg[dp->arg_index].type;
                int flags = dp->flags;
                int has_width;
                size_t width;
                int has_precision;
                size_t precision;

                has_width = 0;
                width = 0;
                if (dp->width_start != dp->width_end)
                  {
                    if (dp->width_arg_index != ARG_NONE)
                      {
                        int arg;

                        if (!(a.arg[dp->width_arg_index].type == TYPE_INT))
                          abort ();
                        arg = a.arg[dp->width_arg_index].a.a_int;
                        width = arg;
                        if (arg < 0)
                          {
                             
                            flags |= FLAG_LEFT;
                            width = -width;
                          }
                      }
                    else
                      {
                        const FCHAR_T *digitp = dp->width_start;

                        do
                          width = xsum (xtimes (width, 10), *digitp++ - '0');
                        while (digitp != dp->width_end);
                      }
                    has_width = 1;
                  }

                has_precision = 0;
                precision = 0;
                if (dp->precision_start != dp->precision_end)
                  {
                    if (dp->precision_arg_index != ARG_NONE)
                      {
                        int arg;

                        if (!(a.arg[dp->precision_arg_index].type == TYPE_INT))
                          abort ();
                        arg = a.arg[dp->precision_arg_index].a.a_int;
                         
                        if (arg >= 0)
                          {
                            precision = arg;
                            has_precision = 1;
                          }
                      }
                    else
                      {
                        const FCHAR_T *digitp = dp->precision_start + 1;

                        precision = 0;
                        while (digitp != dp->precision_end)
                          precision = xsum (xtimes (precision, 10), *digitp++ - '0');
                        has_precision = 1;
                      }
                  }

                switch (type)
                  {
                  case TYPE_U8_STRING:
                    {
                      const uint8_t *arg = a.arg[dp->arg_index].a.a_u8_string;
                      const uint8_t *arg_end;
                      size_t characters;

                      if (has_precision)
                        {
                           
                          arg_end = arg;
                          characters = 0;
                          for (; precision > 0; precision--)
                            {
                              int count = u8_strmblen (arg_end);
                              if (count == 0)
                                break;
                              if (count < 0)
                                {
                                  if (!(result == resultbuf || result == NULL))
                                    free (result);
                                  if (buf_malloced != NULL)
                                    free (buf_malloced);
                                  CLEANUP ();
                                  errno = EILSEQ;
                                  return NULL;
                                }
                              arg_end += count;
                              characters++;
                            }
                        }
                      else if (has_width)
                        {
                           
                          arg_end = arg;
                          characters = 0;
                          for (;;)
                            {
                              int count = u8_strmblen (arg_end);
                              if (count == 0)
                                break;
                              if (count < 0)
                                {
                                  if (!(result == resultbuf || result == NULL))
                                    free (result);
                                  if (buf_malloced != NULL)
                                    free (buf_malloced);
                                  CLEANUP ();
                                  errno = EILSEQ;
                                  return NULL;
                                }
                              arg_end += count;
                              characters++;
                            }
                        }
                      else
                        {
                           
                          arg_end = arg + u8_strlen (arg);
                           
                          characters = 0;
                        }

                      if (characters < width && !(dp->flags & FLAG_LEFT))
                        {
                          size_t n = width - characters;
                          ENSURE_ALLOCATION (xsum (length, n));
                          DCHAR_SET (result + length, ' ', n);
                          length += n;
                        }

# if DCHAR_IS_UINT8_T
                      {
                        size_t n = arg_end - arg;
                        ENSURE_ALLOCATION (xsum (length, n));
                        DCHAR_CPY (result + length, arg, n);
                        length += n;
                      }
# else
                      {  
                        DCHAR_T *converted = result + length;
                        size_t converted_len = allocated - length;
#  if DCHAR_IS_TCHAR
                         
                        converted =
                          u8_conv_to_encoding (locale_charset (),
                                               iconveh_question_mark,
                                               arg, arg_end - arg, NULL,
                                               converted, &converted_len);
#  else
                         
                        converted =
                          U8_TO_DCHAR (arg, arg_end - arg,
                                       converted, &converted_len);
#  endif
                        if (converted == NULL)
                          {
                            int saved_errno = errno;
                            if (!(result == resultbuf || result == NULL))
                              free (result);
                            if (buf_malloced != NULL)
                              free (buf_malloced);
                            CLEANUP ();
                            errno = saved_errno;
                            return NULL;
                          }
                        if (converted != result + length)
                          {
                            ENSURE_ALLOCATION (xsum (length, converted_len));
                            DCHAR_CPY (result + length, converted, converted_len);
                            free (converted);
                          }
                        length += converted_len;
                      }
# endif

                      if (characters < width && (dp->flags & FLAG_LEFT))
                        {
                          size_t n = width - characters;
                          ENSURE_ALLOCATION (xsum (length, n));
                          DCHAR_SET (result + length, ' ', n);
                          length += n;
                        }
                    }
                    break;

                  case TYPE_U16_STRING:
                    {
                      const uint16_t *arg = a.arg[dp->arg_index].a.a_u16_string;
                      const uint16_t *arg_end;
                      size_t characters;

                      if (has_precision)
                        {
                           
                          arg_end = arg;
                          characters = 0;
                          for (; precision > 0; precision--)
                            {
                              int count = u16_strmblen (arg_end);
                              if (count == 0)
                                break;
                              if (count < 0)
                                {
                                  if (!(result == resultbuf || result == NULL))
                                    free (result);
                                  if (buf_malloced != NULL)
                                    free (buf_malloced);
                                  CLEANUP ();
                                  errno = EILSEQ;
                                  return NULL;
                                }
                              arg_end += count;
                              characters++;
                            }
                        }
                      else if (has_width)
                        {
                           
                          arg_end = arg;
                          characters = 0;
                          for (;;)
                            {
                              int count = u16_strmblen (arg_end);
                              if (count == 0)
                                break;
                              if (count < 0)
                                {
                                  if (!(result == resultbuf || result == NULL))
                                    free (result);
                                  if (buf_malloced != NULL)
                                    free (buf_malloced);
                                  CLEANUP ();
                                  errno = EILSEQ;
                                  return NULL;
                                }
                              arg_end += count;
                              characters++;
                            }
                        }
                      else
                        {
                           
                          arg_end = arg + u16_strlen (arg);
                           
                          characters = 0;
                        }

                      if (characters < width && !(dp->flags & FLAG_LEFT))
                        {
                          size_t n = width - characters;
                          ENSURE_ALLOCATION (xsum (length, n));
                          DCHAR_SET (result + length, ' ', n);
                          length += n;
                        }

# if DCHAR_IS_UINT16_T
                      {
                        size_t n = arg_end - arg;
                        ENSURE_ALLOCATION (xsum (length, n));
                        DCHAR_CPY (result + length, arg, n);
                        length += n;
                      }
# else
                      {  
                        DCHAR_T *converted = result + length;
                        size_t converted_len = allocated - length;
#  if DCHAR_IS_TCHAR
                         
                        converted =
                          u16_conv_to_encoding (locale_charset (),
                                                iconveh_question_mark,
                                                arg, arg_end - arg, NULL,
                                                converted, &converted_len);
#  else
                         
                        converted =
                          U16_TO_DCHAR (arg, arg_end - arg,
                                        converted, &converted_len);
#  endif
                        if (converted == NULL)
                          {
                            int saved_errno = errno;
                            if (!(result == resultbuf || result == NULL))
                              free (result);
                            if (buf_malloced != NULL)
                              free (buf_malloced);
                            CLEANUP ();
                            errno = saved_errno;
                            return NULL;
                          }
                        if (converted != result + length)
                          {
                            ENSURE_ALLOCATION (xsum (length, converted_len));
                            DCHAR_CPY (result + length, converted, converted_len);
                            free (converted);
                          }
                        length += converted_len;
                      }
# endif

                      if (characters < width && (dp->flags & FLAG_LEFT))
                        {
                          size_t n = width - characters;
                          ENSURE_ALLOCATION (xsum (length, n));
                          DCHAR_SET (result + length, ' ', n);
                          length += n;
                        }
                    }
                    break;

                  case TYPE_U32_STRING:
                    {
                      const uint32_t *arg = a.arg[dp->arg_index].a.a_u32_string;
                      const uint32_t *arg_end;
                      size_t characters;

                      if (has_precision)
                        {
                           
                          arg_end = arg;
                          characters = 0;
                          for (; precision > 0; precision--)
                            {
                              int count = u32_strmblen (arg_end);
                              if (count == 0)
                                break;
                              if (count < 0)
                                {
                                  if (!(result == resultbuf || result == NULL))
                                    free (result);
                                  if (buf_malloced != NULL)
                                    free (buf_malloced);
                                  CLEANUP ();
                                  errno = EILSEQ;
                                  return NULL;
                                }
                              arg_end += count;
                              characters++;
                            }
                        }
                      else if (has_width)
                        {
                           
                          arg_end = arg;
                          characters = 0;
                          for (;;)
                            {
                              int count = u32_strmblen (arg_end);
                              if (count == 0)
                                break;
                              if (count < 0)
                                {
                                  if (!(result == resultbuf || result == NULL))
                                    free (result);
                                  if (buf_malloced != NULL)
                                    free (buf_malloced);
                                  CLEANUP ();
                                  errno = EILSEQ;
                                  return NULL;
                                }
                              arg_end += count;
                              characters++;
                            }
                        }
                      else
                        {
                           
                          arg_end = arg + u32_strlen (arg);
                           
                          characters = 0;
                        }

                      if (characters < width && !(dp->flags & FLAG_LEFT))
                        {
                          size_t n = width - characters;
                          ENSURE_ALLOCATION (xsum (length, n));
                          DCHAR_SET (result + length, ' ', n);
                          length += n;
                        }

# if DCHAR_IS_UINT32_T
                      {
                        size_t n = arg_end - arg;
                        ENSURE_ALLOCATION (xsum (length, n));
                        DCHAR_CPY (result + length, arg, n);
                        length += n;
                      }
# else
                      {  
                        DCHAR_T *converted = result + length;
                        size_t converted_len = allocated - length;
#  if DCHAR_IS_TCHAR
                         
                        converted =
                          u32_conv_to_encoding (locale_charset (),
                                                iconveh_question_mark,
                                                arg, arg_end - arg, NULL,
                                                converted, &converted_len);
#  else
                         
                        converted =
                          U32_TO_DCHAR (arg, arg_end - arg,
                                        converted, &converted_len);
#  endif
                        if (converted == NULL)
                          {
                            int saved_errno = errno;
                            if (!(result == resultbuf || result == NULL))
                              free (result);
                            if (buf_malloced != NULL)
                              free (buf_malloced);
                            CLEANUP ();
                            errno = saved_errno;
                            return NULL;
                          }
                        if (converted != result + length)
                          {
                            ENSURE_ALLOCATION (xsum (length, converted_len));
                            DCHAR_CPY (result + length, converted, converted_len);
                            free (converted);
                          }
                        length += converted_len;
                      }
# endif

                      if (characters < width && (dp->flags & FLAG_LEFT))
                        {
                          size_t n = width - characters;
                          ENSURE_ALLOCATION (xsum (length, n));
                          DCHAR_SET (result + length, ' ', n);
                          length += n;
                        }
                    }
                    break;

                  default:
                    abort ();
                  }
              }
#endif
#if (!USE_SNPRINTF || !HAVE_SNPRINTF_RETVAL_C99 || USE_MSVC__SNPRINTF || (NEED_PRINTF_DIRECTIVE_LS && !defined IN_LIBINTL)) && HAVE_WCHAR_T
            else if (dp->conversion == 's'
# if WIDE_CHAR_VERSION
                     && a.arg[dp->arg_index].type != TYPE_WIDE_STRING
# else
                     && a.arg[dp->arg_index].type == TYPE_WIDE_STRING
# endif
                    )
              {
                 
                int flags = dp->flags;
                int has_width;
                size_t width;
                int has_precision;
                size_t precision;

                has_width = 0;
                width = 0;
                if (dp->width_start != dp->width_end)
                  {
                    if (dp->width_arg_index != ARG_NONE)
                      {
                        int arg;

                        if (!(a.arg[dp->width_arg_index].type == TYPE_INT))
                          abort ();
                        arg = a.arg[dp->width_arg_index].a.a_int;
                        width = arg;
                        if (arg < 0)
                          {
                             
                            flags |= FLAG_LEFT;
                            width = -width;
                          }
                      }
                    else
                      {
                        const FCHAR_T *digitp = dp->width_start;

                        do
                          width = xsum (xtimes (width, 10), *digitp++ - '0');
                        while (digitp != dp->width_end);
                      }
                    has_width = 1;
                  }

                has_precision = 0;
                precision = 6;
                if (dp->precision_start != dp->precision_end)
                  {
                    if (dp->precision_arg_index != ARG_NONE)
                      {
                        int arg;

                        if (!(a.arg[dp->precision_arg_index].type == TYPE_INT))
                          abort ();
                        arg = a.arg[dp->precision_arg_index].a.a_int;
                         
                        if (arg >= 0)
                          {
                            precision = arg;
                            has_precision = 1;
                          }
                      }
                    else
                      {
                        const FCHAR_T *digitp = dp->precision_start + 1;

                        precision = 0;
                        while (digitp != dp->precision_end)
                          precision = xsum (xtimes (precision, 10), *digitp++ - '0');
                        has_precision = 1;
                      }
                  }

# if WIDE_CHAR_VERSION
                 
                {
                  const char *arg = a.arg[dp->arg_index].a.a_string;
                  const char *arg_end;
                  size_t characters;

                  if (has_precision)
                    {
                       
#  if HAVE_MBRTOWC
                      mbstate_t state;
                      memset (&state, '\0', sizeof (mbstate_t));
#  endif
                      arg_end = arg;
                      characters = 0;
                      for (; precision > 0; precision--)
                        {
                          int count;
#  if HAVE_MBRTOWC
                          count = mbrlen (arg_end, MB_CUR_MAX, &state);
#  else
                          count = mblen (arg_end, MB_CUR_MAX);
#  endif
                          if (count == 0)
                             
                            break;
                          if (count < 0)
                            {
                               
                              if (!(result == resultbuf || result == NULL))
                                free (result);
                              if (buf_malloced != NULL)
                                free (buf_malloced);
                              CLEANUP ();
                              errno = EILSEQ;
                              return NULL;
                            }
                          arg_end += count;
                          characters++;
                        }
                    }
                  else if (has_width)
                    {
                       
#  if HAVE_MBRTOWC
                      mbstate_t state;
                      memset (&state, '\0', sizeof (mbstate_t));
#  endif
                      arg_end = arg;
                      characters = 0;
                      for (;;)
                        {
                          int count;
#  if HAVE_MBRTOWC
                          count = mbrlen (arg_end, MB_CUR_MAX, &state);
#  else
                          count = mblen (arg_end, MB_CUR_MAX);
#  endif
                          if (count == 0)
                             
                            break;
                          if (count < 0)
                            {
                               
                              if (!(result == resultbuf || result == NULL))
                                free (result);
                              if (buf_malloced != NULL)
                                free (buf_malloced);
                              CLEANUP ();
                              errno = EILSEQ;
                              return NULL;
                            }
                          arg_end += count;
                          characters++;
                        }
                    }
                  else
                    {
                       
                      arg_end = arg + strlen (arg);
                       
                      characters = 0;
                    }

                  if (characters < width && !(dp->flags & FLAG_LEFT))
                    {
                      size_t n = width - characters;
                      ENSURE_ALLOCATION (xsum (length, n));
                      DCHAR_SET (result + length, ' ', n);
                      length += n;
                    }

                  if (has_precision || has_width)
                    {
                       
                      size_t remaining;
#  if HAVE_MBRTOWC
                      mbstate_t state;
                      memset (&state, '\0', sizeof (mbstate_t));
#  endif
                      ENSURE_ALLOCATION (xsum (length, characters));
                      for (remaining = characters; remaining > 0; remaining--)
                        {
                          wchar_t wc;
                          int count;
#  if HAVE_MBRTOWC
                          count = mbrtowc (&wc, arg, arg_end - arg, &state);
#  else
                          count = mbtowc (&wc, arg, arg_end - arg);
#  endif
                          if (count <= 0)
                             
                            abort ();
                          result[length++] = wc;
                          arg += count;
                        }
                      if (!(arg == arg_end))
                        abort ();
                    }
                  else
                    {
#  if HAVE_MBRTOWC
                      mbstate_t state;
                      memset (&state, '\0', sizeof (mbstate_t));
#  endif
                      while (arg < arg_end)
                        {
                          wchar_t wc;
                          int count;
#  if HAVE_MBRTOWC
                          count = mbrtowc (&wc, arg, arg_end - arg, &state);
#  else
                          count = mbtowc (&wc, arg, arg_end - arg);
#  endif
                          if (count <= 0)
                             
                            abort ();
                          ENSURE_ALLOCATION (xsum (length, 1));
                          result[length++] = wc;
                          arg += count;
                        }
                    }

                  if (characters < width && (dp->flags & FLAG_LEFT))
                    {
                      size_t n = width - characters;
                      ENSURE_ALLOCATION (xsum (length, n));
                      DCHAR_SET (result + length, ' ', n);
                      length += n;
                    }
                }
# else
                 
                {
                  const wchar_t *arg = a.arg[dp->arg_index].a.a_wide_string;
                  const wchar_t *arg_end;
                  size_t characters;
#  if !DCHAR_IS_TCHAR
                   
                  verify (sizeof (TCHAR_T) == 1);
                  TCHAR_T *tmpsrc;
                  DCHAR_T *tmpdst;
                  size_t tmpdst_len;
#  endif
                  size_t w;

                  if (has_precision)
                    {
                       
#  if HAVE_WCRTOMB && !defined GNULIB_defined_mbstate_t
                      mbstate_t state;
                      memset (&state, '\0', sizeof (mbstate_t));
#  endif
                      arg_end = arg;
                      characters = 0;
                      while (precision > 0)
                        {
                          char cbuf[64];  
                          int count;

                          if (*arg_end == 0)
                             
                            break;
#  if HAVE_WCRTOMB && !defined GNULIB_defined_mbstate_t
                          count = wcrtomb (cbuf, *arg_end, &state);
#  else
                          count = wctomb (cbuf, *arg_end);
#  endif
                          if (count < 0)
                            {
                               
                              if (!(result == resultbuf || result == NULL))
                                free (result);
                              if (buf_malloced != NULL)
                                free (buf_malloced);
                              CLEANUP ();
                              errno = EILSEQ;
                              return NULL;
                            }
                          if (precision < count)
                            break;
                          arg_end++;
                          characters += count;
                          precision -= count;
                        }
                    }
#  if DCHAR_IS_TCHAR
                  else if (has_width)
#  else
                  else
#  endif
                    {
                       
#  if HAVE_WCRTOMB && !defined GNULIB_defined_mbstate_t
                      mbstate_t state;
                      memset (&state, '\0', sizeof (mbstate_t));
#  endif
                      arg_end = arg;
                      characters = 0;
                      for (;;)
                        {
                          char cbuf[64];  
                          int count;

                          if (*arg_end == 0)
                             
                            break;
#  if HAVE_WCRTOMB && !defined GNULIB_defined_mbstate_t
                          count = wcrtomb (cbuf, *arg_end, &state);
#  else
                          count = wctomb (cbuf, *arg_end);
#  endif
                          if (count < 0)
                            {
                               
                              if (!(result == resultbuf || result == NULL))
                                free (result);
                              if (buf_malloced != NULL)
                                free (buf_malloced);
                              CLEANUP ();
                              errno = EILSEQ;
                              return NULL;
                            }
                          arg_end++;
                          characters += count;
                        }
                    }
#  if DCHAR_IS_TCHAR
                  else
                    {
                       
                      arg_end = arg + local_wcslen (arg);
                       
                      characters = 0;
                    }
#  endif

#  if !DCHAR_IS_TCHAR
                   
                  tmpsrc = (TCHAR_T *) malloc (characters * sizeof (TCHAR_T));
                  if (tmpsrc == NULL)
                    goto out_of_memory;
                  {
                    TCHAR_T *tmpptr = tmpsrc;
                    size_t remaining;
#   if HAVE_WCRTOMB && !defined GNULIB_defined_mbstate_t
                    mbstate_t state;
                    memset (&state, '\0', sizeof (mbstate_t));
#   endif
                    for (remaining = characters; remaining > 0; )
                      {
                        char cbuf[64];  
                        int count;

                        if (*arg == 0)
                          abort ();
#   if HAVE_WCRTOMB && !defined GNULIB_defined_mbstate_t
                        count = wcrtomb (cbuf, *arg, &state);
#   else
                        count = wctomb (cbuf, *arg);
#   endif
                        if (count <= 0)
                           
                          abort ();
                        memcpy (tmpptr, cbuf, count);
                        tmpptr += count;
                        arg++;
                        remaining -= count;
                      }
                    if (!(arg == arg_end))
                      abort ();
                  }

                   
                  tmpdst =
                    DCHAR_CONV_FROM_ENCODING (locale_charset (),
                                              iconveh_question_mark,
                                              tmpsrc, characters,
                                              NULL,
                                              NULL, &tmpdst_len);
                  if (tmpdst == NULL)
                    {
                      int saved_errno = errno;
                      free (tmpsrc);
                      if (!(result == resultbuf || result == NULL))
                        free (result);
                      if (buf_malloced != NULL)
                        free (buf_malloced);
                      CLEANUP ();
                      errno = saved_errno;
                      return NULL;
                    }
                  free (tmpsrc);
#  endif

                  if (has_width)
                    {
#  if ENABLE_UNISTDIO
                       
                      w = DCHAR_MBSNLEN (result + length, characters);
#  else
                       
                      w = characters;
#  endif
                    }
                  else
                     
                    w = 0;

                  if (w < width && !(dp->flags & FLAG_LEFT))
                    {
                      size_t n = width - w;
                      ENSURE_ALLOCATION (xsum (length, n));
                      DCHAR_SET (result + length, ' ', n);
                      length += n;
                    }

#  if DCHAR_IS_TCHAR
                  if (has_precision || has_width)
                    {
                       
                      size_t remaining;
#   if HAVE_WCRTOMB && !defined GNULIB_defined_mbstate_t
                      mbstate_t state;
                      memset (&state, '\0', sizeof (mbstate_t));
#   endif
                      ENSURE_ALLOCATION (xsum (length, characters));
                      for (remaining = characters; remaining > 0; )
                        {
                          char cbuf[64];  
                          int count;

                          if (*arg == 0)
                            abort ();
#   if HAVE_WCRTOMB && !defined GNULIB_defined_mbstate_t
                          count = wcrtomb (cbuf, *arg, &state);
#   else
                          count = wctomb (cbuf, *arg);
#   endif
                          if (count <= 0)
                             
                            abort ();
                          memcpy (result + length, cbuf, count);
                          length += count;
                          arg++;
                          remaining -= count;
                        }
                      if (!(arg == arg_end))
                        abort ();
                    }
                  else
                    {
#   if HAVE_WCRTOMB && !defined GNULIB_defined_mbstate_t
                      mbstate_t state;
                      memset (&state, '\0', sizeof (mbstate_t));
#   endif
                      while (arg < arg_end)
                        {
                          char cbuf[64];  
                          int count;

                          if (*arg == 0)
                            abort ();
#   if HAVE_WCRTOMB && !defined GNULIB_defined_mbstate_t
                          count = wcrtomb (cbuf, *arg, &state);
#   else
                          count = wctomb (cbuf, *arg);
#   endif
                          if (count <= 0)
                            {
                               
                              if (!(result == resultbuf || result == NULL))
                                free (result);
                              if (buf_malloced != NULL)
                                free (buf_malloced);
                              CLEANUP ();
                              errno = EILSEQ;
                              return NULL;
                            }
                          ENSURE_ALLOCATION (xsum (length, count));
                          memcpy (result + length, cbuf, count);
                          length += count;
                          arg++;
                        }
                    }
#  else
                  ENSURE_ALLOCATION (xsum (length, tmpdst_len));
                  DCHAR_CPY (result + length, tmpdst, tmpdst_len);
                  free (tmpdst);
                  length += tmpdst_len;
#  endif

                  if (w < width && (dp->flags & FLAG_LEFT))
                    {
                      size_t n = width - w;
                      ENSURE_ALLOCATION (xsum (length, n));
                      DCHAR_SET (result + length, ' ', n);
                      length += n;
                    }
                }
# endif
              }
#endif
#if (NEED_PRINTF_DIRECTIVE_A || NEED_PRINTF_LONG_DOUBLE || NEED_PRINTF_DOUBLE) && !defined IN_LIBINTL
            else if ((dp->conversion == 'a' || dp->conversion == 'A')
# if !(NEED_PRINTF_DIRECTIVE_A || (NEED_PRINTF_LONG_DOUBLE && NEED_PRINTF_DOUBLE))
                     && (0
#  if NEED_PRINTF_DOUBLE
                         || a.arg[dp->arg_index].type == TYPE_DOUBLE
#  endif
#  if NEED_PRINTF_LONG_DOUBLE
                         || a.arg[dp->arg_index].type == TYPE_LONGDOUBLE
#  endif
                        )
# endif
                    )
              {
                arg_type type = a.arg[dp->arg_index].type;
                int flags = dp->flags;
                size_t width;
                int has_precision;
                size_t precision;
                size_t tmp_length;
                size_t count;
                DCHAR_T tmpbuf[700];
                DCHAR_T *tmp;
                DCHAR_T *pad_ptr;
                DCHAR_T *p;

                width = 0;
                if (dp->width_start != dp->width_end)
                  {
                    if (dp->width_arg_index != ARG_NONE)
                      {
                        int arg;

                        if (!(a.arg[dp->width_arg_index].type == TYPE_INT))
                          abort ();
                        arg = a.arg[dp->width_arg_index].a.a_int;
                        width = arg;
                        if (arg < 0)
                          {
                             
                            flags |= FLAG_LEFT;
                            width = -width;
                          }
                      }
                    else
                      {
                        const FCHAR_T *digitp = dp->width_start;

                        do
                          width = xsum (xtimes (width, 10), *digitp++ - '0');
                        while (digitp != dp->width_end);
                      }
                  }

                has_precision = 0;
                precision = 0;
                if (dp->precision_start != dp->precision_end)
                  {
                    if (dp->precision_arg_index != ARG_NONE)
                      {
                        int arg;

                        if (!(a.arg[dp->precision_arg_index].type == TYPE_INT))
                          abort ();
                        arg = a.arg[dp->precision_arg_index].a.a_int;
                         
                        if (arg >= 0)
                          {
                            precision = arg;
                            has_precision = 1;
                          }
                      }
                    else
                      {
                        const FCHAR_T *digitp = dp->precision_start + 1;

                        precision = 0;
                        while (digitp != dp->precision_end)
                          precision = xsum (xtimes (precision, 10), *digitp++ - '0');
                        has_precision = 1;
                      }
                  }

                 
                if (type == TYPE_LONGDOUBLE)
                  tmp_length =
                    (unsigned int) ((LDBL_DIG + 1)
                                    * 0.831  
                                   )
                    + 1;  
                else
                  tmp_length =
                    (unsigned int) ((DBL_DIG + 1)
                                    * 0.831  
                                   )
                    + 1;  
                if (tmp_length < precision)
                  tmp_length = precision;
                 
                tmp_length = xsum (tmp_length, 12);

                if (tmp_length < width)
                  tmp_length = width;

                tmp_length = xsum (tmp_length, 1);  

                if (tmp_length <= sizeof (tmpbuf) / sizeof (DCHAR_T))
                  tmp = tmpbuf;
                else
                  {
                    size_t tmp_memsize = xtimes (tmp_length, sizeof (DCHAR_T));

                    if (size_overflow_p (tmp_memsize))
                       
                      goto out_of_memory;
                    tmp = (DCHAR_T *) malloc (tmp_memsize);
                    if (tmp == NULL)
                       
                      goto out_of_memory;
                  }

                pad_ptr = NULL;
                p = tmp;
                if (type == TYPE_LONGDOUBLE)
                  {
# if NEED_PRINTF_DIRECTIVE_A || NEED_PRINTF_LONG_DOUBLE
                    long double arg = a.arg[dp->arg_index].a.a_longdouble;

                    if (isnanl (arg))
                      {
                        if (dp->conversion == 'A')
                          {
                            *p++ = 'N'; *p++ = 'A'; *p++ = 'N';
                          }
                        else
                          {
                            *p++ = 'n'; *p++ = 'a'; *p++ = 'n';
                          }
                      }
                    else
                      {
                        int sign = 0;
                        DECL_LONG_DOUBLE_ROUNDING

                        BEGIN_LONG_DOUBLE_ROUNDING ();

                        if (signbit (arg))  
                          {
                            sign = -1;
                            arg = -arg;
                          }

                        if (sign < 0)
                          *p++ = '-';
                        else if (flags & FLAG_SHOWSIGN)
                          *p++ = '+';
                        else if (flags & FLAG_SPACE)
                          *p++ = ' ';

                        if (arg > 0.0L && arg + arg == arg)
                          {
                            if (dp->conversion == 'A')
                              {
                                *p++ = 'I'; *p++ = 'N'; *p++ = 'F';
                              }
                            else
                              {
                                *p++ = 'i'; *p++ = 'n'; *p++ = 'f';
                              }
                          }
                        else
                          {
                            int exponent;
                            long double mantissa;

                            if (arg > 0.0L)
                              mantissa = printf_frexpl (arg, &exponent);
                            else
                              {
                                exponent = 0;
                                mantissa = 0.0L;
                              }

                            if (has_precision
                                && precision < (unsigned int) ((LDBL_DIG + 1) * 0.831) + 1)
                              {
                                 
                                long double tail = mantissa;
                                size_t q;

                                for (q = precision; ; q--)
                                  {
                                    int digit = (int) tail;
                                    tail -= digit;
                                    if (q == 0)
                                      {
                                        if (digit & 1 ? tail >= 0.5L : tail > 0.5L)
                                          tail = 1 - tail;
                                        else
                                          tail = - tail;
                                        break;
                                      }
                                    tail *= 16.0L;
                                  }
                                if (tail != 0.0L)
                                  for (q = precision; q > 0; q--)
                                    tail *= 0.0625L;
                                mantissa += tail;
                              }

                            *p++ = '0';
                            *p++ = dp->conversion - 'A' + 'X';
                            pad_ptr = p;
                            {
                              int digit;

                              digit = (int) mantissa;
                              mantissa -= digit;
                              *p++ = '0' + digit;
                              if ((flags & FLAG_ALT)
                                  || mantissa > 0.0L || precision > 0)
                                {
                                  *p++ = decimal_point_char ();
                                   
                                  while (mantissa > 0.0L)
                                    {
                                      mantissa *= 16.0L;
                                      digit = (int) mantissa;
                                      mantissa -= digit;
                                      *p++ = digit
                                             + (digit < 10
                                                ? '0'
                                                : dp->conversion - 10);
                                      if (precision > 0)
                                        precision--;
                                    }
                                  while (precision > 0)
                                    {
                                      *p++ = '0';
                                      precision--;
                                    }
                                }
                              }
                              *p++ = dp->conversion - 'A' + 'P';
#  if WIDE_CHAR_VERSION
                              {
                                static const wchar_t decimal_format[] =
                                  { '%', '+', 'd', '\0' };
                                SNPRINTF (p, 6 + 1, decimal_format, exponent);
                              }
                              while (*p != '\0')
                                p++;
#  else
                              if (sizeof (DCHAR_T) == 1)
                                {
                                  sprintf ((char *) p, "%+d", exponent);
                                  while (*p != '\0')
                                    p++;
                                }
                              else
                                {
                                  char expbuf[6 + 1];
                                  const char *ep;
                                  sprintf (expbuf, "%+d", exponent);
                                  for (ep = expbuf; (*p = *ep) != '\0'; ep++)
                                    p++;
                                }
#  endif
                          }

                        END_LONG_DOUBLE_ROUNDING ();
                      }
# else
                    abort ();
# endif
                  }
                else
                  {
# if NEED_PRINTF_DIRECTIVE_A || NEED_PRINTF_DOUBLE
                    double arg = a.arg[dp->arg_index].a.a_double;

                    if (isnand (arg))
                      {
                        if (dp->conversion == 'A')
                          {
                            *p++ = 'N'; *p++ = 'A'; *p++ = 'N';
                          }
                        else
                          {
                            *p++ = 'n'; *p++ = 'a'; *p++ = 'n';
                          }
                      }
                    else
                      {
                        int sign = 0;

                        if (signbit (arg))  
                          {
                            sign = -1;
                            arg = -arg;
                          }

                        if (sign < 0)
                          *p++ = '-';
                        else if (flags & FLAG_SHOWSIGN)
                          *p++ = '+';
                        else if (flags & FLAG_SPACE)
                          *p++ = ' ';

                        if (arg > 0.0 && arg + arg == arg)
                          {
                            if (dp->conversion == 'A')
                              {
                                *p++ = 'I'; *p++ = 'N'; *p++ = 'F';
                              }
                            else
                              {
                                *p++ = 'i'; *p++ = 'n'; *p++ = 'f';
                              }
                          }
                        else
                          {
                            int exponent;
                            double mantissa;

                            if (arg > 0.0)
                              mantissa = printf_frexp (arg, &exponent);
                            else
                              {
                                exponent = 0;
                                mantissa = 0.0;
                              }

                            if (has_precision
                                && precision < (unsigned int) ((DBL_DIG + 1) * 0.831) + 1)
                              {
                                 
                                double tail = mantissa;
                                size_t q;

                                for (q = precision; ; q--)
                                  {
                                    int digit = (int) tail;
                                    tail -= digit;
                                    if (q == 0)
                                      {
                                        if (digit & 1 ? tail >= 0.5 : tail > 0.5)
                                          tail = 1 - tail;
                                        else
                                          tail = - tail;
                                        break;
                                      }
                                    tail *= 16.0;
                                  }
                                if (tail != 0.0)
                                  for (q = precision; q > 0; q--)
                                    tail *= 0.0625;
                                mantissa += tail;
                              }

                            *p++ = '0';
                            *p++ = dp->conversion - 'A' + 'X';
                            pad_ptr = p;
                            {
                              int digit;

                              digit = (int) mantissa;
                              mantissa -= digit;
                              *p++ = '0' + digit;
                              if ((flags & FLAG_ALT)
                                  || mantissa > 0.0 || precision > 0)
                                {
                                  *p++ = decimal_point_char ();
                                   
                                  while (mantissa > 0.0)
                                    {
                                      mantissa *= 16.0;
                                      digit = (int) mantissa;
                                      mantissa -= digit;
                                      *p++ = digit
                                             + (digit < 10
                                                ? '0'
                                                : dp->conversion - 10);
                                      if (precision > 0)
                                        precision--;
                                    }
                                  while (precision > 0)
                                    {
                                      *p++ = '0';
                                      precision--;
                                    }
                                }
                              }
                              *p++ = dp->conversion - 'A' + 'P';
#  if WIDE_CHAR_VERSION
                              {
                                static const wchar_t decimal_format[] =
                                  { '%', '+', 'd', '\0' };
                                SNPRINTF (p, 6 + 1, decimal_format, exponent);
                              }
                              while (*p != '\0')
                                p++;
#  else
                              if (sizeof (DCHAR_T) == 1)
                                {
                                  sprintf ((char *) p, "%+d", exponent);
                                  while (*p != '\0')
                                    p++;
                                }
                              else
                                {
                                  char expbuf[6 + 1];
                                  const char *ep;
                                  sprintf (expbuf, "%+d", exponent);
                                  for (ep = expbuf; (*p = *ep) != '\0'; ep++)
                                    p++;
                                }
#  endif
                          }
                      }
# else
                    abort ();
# endif
                  }

                 
                count = p - tmp;

                if (count < width)
                  {
                    size_t pad = width - count;
                    DCHAR_T *end = p + pad;

                    if (flags & FLAG_LEFT)
                      {
                         
                        for (; pad > 0; pad--)
                          *p++ = ' ';
                      }
                    else if ((flags & FLAG_ZERO) && pad_ptr != NULL)
                      {
                         
                        DCHAR_T *q = end;

                        while (p > pad_ptr)
                          *--q = *--p;
                        for (; pad > 0; pad--)
                          *p++ = '0';
                      }
                    else
                      {
                         
                        DCHAR_T *q = end;

                        while (p > tmp)
                          *--q = *--p;
                        for (; pad > 0; pad--)
                          *p++ = ' ';
                      }

                    p = end;
                  }

                count = p - tmp;

                if (count >= tmp_length)
                   
                  abort ();

                 
                if (count >= allocated - length)
                  {
                    size_t n = xsum (length, count);

                    ENSURE_ALLOCATION (n);
                  }

                 
                memcpy (result + length, tmp, count * sizeof (DCHAR_T));
                if (tmp != tmpbuf)
                  free (tmp);
                length += count;
              }
#endif
#if (NEED_PRINTF_INFINITE_DOUBLE || NEED_PRINTF_DOUBLE || NEED_PRINTF_INFINITE_LONG_DOUBLE || NEED_PRINTF_LONG_DOUBLE) && !defined IN_LIBINTL
            else if ((dp->conversion == 'f' || dp->conversion == 'F'
                      || dp->conversion == 'e' || dp->conversion == 'E'
                      || dp->conversion == 'g' || dp->conversion == 'G'
                      || dp->conversion == 'a' || dp->conversion == 'A')
                     && (0
# if NEED_PRINTF_DOUBLE
                         || a.arg[dp->arg_index].type == TYPE_DOUBLE
# elif NEED_PRINTF_INFINITE_DOUBLE
                         || (a.arg[dp->arg_index].type == TYPE_DOUBLE
                              
                             && is_infinite_or_zero (a.arg[dp->arg_index].a.a_double))
# endif
# if NEED_PRINTF_LONG_DOUBLE
                         || a.arg[dp->arg_index].type == TYPE_LONGDOUBLE
# elif NEED_PRINTF_INFINITE_LONG_DOUBLE
                         || (a.arg[dp->arg_index].type == TYPE_LONGDOUBLE
                              
                             && is_infinite_or_zerol (a.arg[dp->arg_index].a.a_longdouble))
# endif
                        ))
              {
# if (NEED_PRINTF_DOUBLE || NEED_PRINTF_INFINITE_DOUBLE) && (NEED_PRINTF_LONG_DOUBLE || NEED_PRINTF_INFINITE_LONG_DOUBLE)
                arg_type type = a.arg[dp->arg_index].type;
# endif
                int flags = dp->flags;
                size_t width;
                size_t count;
                int has_precision;
                size_t precision;
                size_t tmp_length;
                DCHAR_T tmpbuf[700];
                DCHAR_T *tmp;
                DCHAR_T *pad_ptr;
                DCHAR_T *p;

                width = 0;
                if (dp->width_start != dp->width_end)
                  {
                    if (dp->width_arg_index != ARG_NONE)
                      {
                        int arg;

                        if (!(a.arg[dp->width_arg_index].type == TYPE_INT))
                          abort ();
                        arg = a.arg[dp->width_arg_index].a.a_int;
                        width = arg;
                        if (arg < 0)
                          {
                             
                            flags |= FLAG_LEFT;
                            width = -width;
                          }
                      }
                    else
                      {
                        const FCHAR_T *digitp = dp->width_start;

                        do
                          width = xsum (xtimes (width, 10), *digitp++ - '0');
                        while (digitp != dp->width_end);
                      }
                  }

                has_precision = 0;
                precision = 0;
                if (dp->precision_start != dp->precision_end)
                  {
                    if (dp->precision_arg_index != ARG_NONE)
                      {
                        int arg;

                        if (!(a.arg[dp->precision_arg_index].type == TYPE_INT))
                          abort ();
                        arg = a.arg[dp->precision_arg_index].a.a_int;
                         
                        if (arg >= 0)
                          {
                            precision = arg;
                            has_precision = 1;
                          }
                      }
                    else
                      {
                        const FCHAR_T *digitp = dp->precision_start + 1;

                        precision = 0;
                        while (digitp != dp->precision_end)
                          precision = xsum (xtimes (precision, 10), *digitp++ - '0');
                        has_precision = 1;
                      }
                  }

                 
                if (!has_precision)
                  if (!(dp->conversion == 'a' || dp->conversion == 'A'))
                    precision = 6;

                 
# if NEED_PRINTF_DOUBLE && NEED_PRINTF_LONG_DOUBLE
                tmp_length = (type == TYPE_LONGDOUBLE ? LDBL_DIG + 1 : DBL_DIG + 1);
# elif NEED_PRINTF_INFINITE_DOUBLE && NEED_PRINTF_LONG_DOUBLE
                tmp_length = (type == TYPE_LONGDOUBLE ? LDBL_DIG + 1 : 0);
# elif NEED_PRINTF_LONG_DOUBLE
                tmp_length = LDBL_DIG + 1;
# elif NEED_PRINTF_DOUBLE
                tmp_length = DBL_DIG + 1;
# else
                tmp_length = 0;
# endif
                if (tmp_length < precision)
                  tmp_length = precision;
# if NEED_PRINTF_LONG_DOUBLE
#  if NEED_PRINTF_DOUBLE || NEED_PRINTF_INFINITE_DOUBLE
                if (type == TYPE_LONGDOUBLE)
#  endif
                  if (dp->conversion == 'f' || dp->conversion == 'F')
                    {
                      long double arg = a.arg[dp->arg_index].a.a_longdouble;
                      if (!(isnanl (arg) || arg + arg == arg))
                        {
                           
                          int exponent = floorlog10l (arg < 0 ? -arg : arg);
                          if (exponent >= 0 && tmp_length < exponent + precision)
                            tmp_length = exponent + precision;
                        }
                    }
# endif
# if NEED_PRINTF_DOUBLE
#  if NEED_PRINTF_LONG_DOUBLE || NEED_PRINTF_INFINITE_LONG_DOUBLE
                if (type == TYPE_DOUBLE)
#  endif
                  if (dp->conversion == 'f' || dp->conversion == 'F')
                    {
                      double arg = a.arg[dp->arg_index].a.a_double;
                      if (!(isnand (arg) || arg + arg == arg))
                        {
                           
                          int exponent = floorlog10 (arg < 0 ? -arg : arg);
                          if (exponent >= 0 && tmp_length < exponent + precision)
                            tmp_length = exponent + precision;
                        }
                    }
# endif
                 
                tmp_length = xsum (tmp_length, 12);

                if (tmp_length < width)
                  tmp_length = width;

                tmp_length = xsum (tmp_length, 1);  

                if (tmp_length <= sizeof (tmpbuf) / sizeof (DCHAR_T))
                  tmp = tmpbuf;
                else
                  {
                    size_t tmp_memsize = xtimes (tmp_length, sizeof (DCHAR_T));

                    if (size_overflow_p (tmp_memsize))
                       
                      goto out_of_memory;
                    tmp = (DCHAR_T *) malloc (tmp_memsize);
                    if (tmp == NULL)
                       
                      goto out_of_memory;
                  }

                pad_ptr = NULL;
                p = tmp;

# if NEED_PRINTF_LONG_DOUBLE || NEED_PRINTF_INFINITE_LONG_DOUBLE
#  if NEED_PRINTF_DOUBLE || NEED_PRINTF_INFINITE_DOUBLE
                if (type == TYPE_LONGDOUBLE)
#  endif
                  {
                    long double arg = a.arg[dp->arg_index].a.a_longdouble;

                    if (isnanl (arg))
                      {
                        if (dp->conversion >= 'A' && dp->conversion <= 'Z')
                          {
                            *p++ = 'N'; *p++ = 'A'; *p++ = 'N';
                          }
                        else
                          {
                            *p++ = 'n'; *p++ = 'a'; *p++ = 'n';
                          }
                      }
                    else
                      {
                        int sign = 0;
                        DECL_LONG_DOUBLE_ROUNDING

                        BEGIN_LONG_DOUBLE_ROUNDING ();

                        if (signbit (arg))  
                          {
                            sign = -1;
                            arg = -arg;
                          }

                        if (sign < 0)
                          *p++ = '-';
                        else if (flags & FLAG_SHOWSIGN)
                          *p++ = '+';
                        else if (flags & FLAG_SPACE)
                          *p++ = ' ';

                        if (arg > 0.0L && arg + arg == arg)
                          {
                            if (dp->conversion >= 'A' && dp->conversion <= 'Z')
                              {
                                *p++ = 'I'; *p++ = 'N'; *p++ = 'F';
                              }
                            else
                              {
                                *p++ = 'i'; *p++ = 'n'; *p++ = 'f';
                              }
                          }
                        else
                          {
#  if NEED_PRINTF_LONG_DOUBLE
                            pad_ptr = p;

                            if (dp->conversion == 'f' || dp->conversion == 'F')
                              {
                                char *digits;
                                size_t ndigits;

                                digits =
                                  scale10_round_decimal_long_double (arg, precision);
                                if (digits == NULL)
                                  {
                                    END_LONG_DOUBLE_ROUNDING ();
                                    goto out_of_memory;
                                  }
                                ndigits = strlen (digits);

                                if (ndigits > precision)
                                  do
                                    {
                                      --ndigits;
                                      *p++ = digits[ndigits];
                                    }
                                  while (ndigits > precision);
                                else
                                  *p++ = '0';
                                 
                                if ((flags & FLAG_ALT) || precision > 0)
                                  {
                                    *p++ = decimal_point_char ();
                                    for (; precision > ndigits; precision--)
                                      *p++ = '0';
                                    while (ndigits > 0)
                                      {
                                        --ndigits;
                                        *p++ = digits[ndigits];
                                      }
                                  }

                                free (digits);
                              }
                            else if (dp->conversion == 'e' || dp->conversion == 'E')
                              {
                                int exponent;

                                if (arg == 0.0L)
                                  {
                                    exponent = 0;
                                    *p++ = '0';
                                    if ((flags & FLAG_ALT) || precision > 0)
                                      {
                                        *p++ = decimal_point_char ();
                                        for (; precision > 0; precision--)
                                          *p++ = '0';
                                      }
                                  }
                                else
                                  {
                                     
                                    int adjusted;
                                    char *digits;
                                    size_t ndigits;

                                    exponent = floorlog10l (arg);
                                    adjusted = 0;
                                    for (;;)
                                      {
                                        digits =
                                          scale10_round_decimal_long_double (arg,
                                                                             (int)precision - exponent);
                                        if (digits == NULL)
                                          {
                                            END_LONG_DOUBLE_ROUNDING ();
                                            goto out_of_memory;
                                          }
                                        ndigits = strlen (digits);

                                        if (ndigits == precision + 1)
                                          break;
                                        if (ndigits < precision
                                            || ndigits > precision + 2)
                                           
                                          abort ();
                                        if (adjusted)
                                           
                                          abort ();
                                        free (digits);
                                        if (ndigits == precision)
                                          exponent -= 1;
                                        else
                                          exponent += 1;
                                        adjusted = 1;
                                      }
                                     
                                    if (is_borderline (digits, precision))
                                      {
                                         
                                        char *digits2 =
                                          scale10_round_decimal_long_double (arg,
                                                                             (int)precision - exponent + 1);
                                        if (digits2 == NULL)
                                          {
                                            free (digits);
                                            END_LONG_DOUBLE_ROUNDING ();
                                            goto out_of_memory;
                                          }
                                        if (strlen (digits2) == precision + 1)
                                          {
                                            free (digits);
                                            digits = digits2;
                                            exponent -= 1;
                                          }
                                        else
                                          free (digits2);
                                      }
                                     

                                    *p++ = digits[--ndigits];
                                    if ((flags & FLAG_ALT) || precision > 0)
                                      {
                                        *p++ = decimal_point_char ();
                                        while (ndigits > 0)
                                          {
                                            --ndigits;
                                            *p++ = digits[ndigits];
                                          }
                                      }

                                    free (digits);
                                  }

                                *p++ = dp->conversion;  
#   if WIDE_CHAR_VERSION
                                {
                                  static const wchar_t decimal_format[] =
                                    { '%', '+', '.', '2', 'd', '\0' };
                                  SNPRINTF (p, 6 + 1, decimal_format, exponent);
                                }
                                while (*p != '\0')
                                  p++;
#   else
                                if (sizeof (DCHAR_T) == 1)
                                  {
                                    sprintf ((char *) p, "%+.2d", exponent);
                                    while (*p != '\0')
                                      p++;
                                  }
                                else
                                  {
                                    char expbuf[6 + 1];
                                    const char *ep;
                                    sprintf (expbuf, "%+.2d", exponent);
                                    for (ep = expbuf; (*p = *ep) != '\0'; ep++)
                                      p++;
                                  }
#   endif
                              }
                            else if (dp->conversion == 'g' || dp->conversion == 'G')
                              {
                                if (precision == 0)
                                  precision = 1;
                                 

                                if (arg == 0.0L)
                                   
                                  {
                                    size_t ndigits = precision;
                                     
                                    size_t nzeroes =
                                      (flags & FLAG_ALT ? 0 : precision - 1);

                                    --ndigits;
                                    *p++ = '0';
                                    if ((flags & FLAG_ALT) || ndigits > nzeroes)
                                      {
                                        *p++ = decimal_point_char ();
                                        while (ndigits > nzeroes)
                                          {
                                            --ndigits;
                                            *p++ = '0';
                                          }
                                      }
                                  }
                                else
                                  {
                                     
                                    int exponent;
                                    int adjusted;
                                    char *digits;
                                    size_t ndigits;
                                    size_t nzeroes;

                                    exponent = floorlog10l (arg);
                                    adjusted = 0;
                                    for (;;)
                                      {
                                        digits =
                                          scale10_round_decimal_long_double (arg,
                                                                             (int)(precision - 1) - exponent);
                                        if (digits == NULL)
                                          {
                                            END_LONG_DOUBLE_ROUNDING ();
                                            goto out_of_memory;
                                          }
                                        ndigits = strlen (digits);

                                        if (ndigits == precision)
                                          break;
                                        if (ndigits < precision - 1
                                            || ndigits > precision + 1)
                                           
                                          abort ();
                                        if (adjusted)
                                           
                                          abort ();
                                        free (digits);
                                        if (ndigits < precision)
                                          exponent -= 1;
                                        else
                                          exponent += 1;
                                        adjusted = 1;
                                      }
                                     
                                    if (is_borderline (digits, precision - 1))
                                      {
                                         
                                        char *digits2 =
                                          scale10_round_decimal_long_double (arg,
                                                                             (int)(precision - 1) - exponent + 1);
                                        if (digits2 == NULL)
                                          {
                                            free (digits);
                                            END_LONG_DOUBLE_ROUNDING ();
                                            goto out_of_memory;
                                          }
                                        if (strlen (digits2) == precision)
                                          {
                                            free (digits);
                                            digits = digits2;
                                            exponent -= 1;
                                          }
                                        else
                                          free (digits2);
                                      }
                                     

                                     
                                    nzeroes = 0;
                                    if ((flags & FLAG_ALT) == 0)
                                      while (nzeroes < ndigits
                                             && digits[nzeroes] == '0')
                                        nzeroes++;

                                     
                                    if (exponent >= -4
                                        && exponent < (long)precision)
                                      {
                                         
                                        if (exponent >= 0)
                                          {
                                            size_t ecount = exponent + 1;
                                             
                                            for (; ecount > 0; ecount--)
                                              *p++ = digits[--ndigits];
                                            if ((flags & FLAG_ALT) || ndigits > nzeroes)
                                              {
                                                *p++ = decimal_point_char ();
                                                while (ndigits > nzeroes)
                                                  {
                                                    --ndigits;
                                                    *p++ = digits[ndigits];
                                                  }
                                              }
                                          }
                                        else
                                          {
                                            size_t ecount = -exponent - 1;
                                            *p++ = '0';
                                            *p++ = decimal_point_char ();
                                            for (; ecount > 0; ecount--)
                                              *p++ = '0';
                                            while (ndigits > nzeroes)
                                              {
                                                --ndigits;
                                                *p++ = digits[ndigits];
                                              }
                                          }
                                      }
                                    else
                                      {
                                         
                                        *p++ = digits[--ndigits];
                                        if ((flags & FLAG_ALT) || ndigits > nzeroes)
                                          {
                                            *p++ = decimal_point_char ();
                                            while (ndigits > nzeroes)
                                              {
                                                --ndigits;
                                                *p++ = digits[ndigits];
                                              }
                                          }
                                        *p++ = dp->conversion - 'G' + 'E';  
#   if WIDE_CHAR_VERSION
                                        {
                                          static const wchar_t decimal_format[] =
                                            { '%', '+', '.', '2', 'd', '\0' };
                                          SNPRINTF (p, 6 + 1, decimal_format, exponent);
                                        }
                                        while (*p != '\0')
                                          p++;
#   else
                                        if (sizeof (DCHAR_T) == 1)
                                          {
                                            sprintf ((char *) p, "%+.2d", exponent);
                                            while (*p != '\0')
                                              p++;
                                          }
                                        else
                                          {
                                            char expbuf[6 + 1];
                                            const char *ep;
                                            sprintf (expbuf, "%+.2d", exponent);
                                            for (ep = expbuf; (*p = *ep) != '\0'; ep++)
                                              p++;
                                          }
#   endif
                                      }

                                    free (digits);
                                  }
                              }
                            else
                              abort ();
#  else
                             
                            if (!(arg == 0.0L))
                              abort ();

                            pad_ptr = p;

                            if (dp->conversion == 'f' || dp->conversion == 'F')
                              {
                                *p++ = '0';
                                if ((flags & FLAG_ALT) || precision > 0)
                                  {
                                    *p++ = decimal_point_char ();
                                    for (; precision > 0; precision--)
                                      *p++ = '0';
                                  }
                              }
                            else if (dp->conversion == 'e' || dp->conversion == 'E')
                              {
                                *p++ = '0';
                                if ((flags & FLAG_ALT) || precision > 0)
                                  {
                                    *p++ = decimal_point_char ();
                                    for (; precision > 0; precision--)
                                      *p++ = '0';
                                  }
                                *p++ = dp->conversion;  
                                *p++ = '+';
                                *p++ = '0';
                                *p++ = '0';
                              }
                            else if (dp->conversion == 'g' || dp->conversion == 'G')
                              {
                                *p++ = '0';
                                if (flags & FLAG_ALT)
                                  {
                                    size_t ndigits =
                                      (precision > 0 ? precision - 1 : 0);
                                    *p++ = decimal_point_char ();
                                    for (; ndigits > 0; --ndigits)
                                      *p++ = '0';
                                  }
                              }
                            else if (dp->conversion == 'a' || dp->conversion == 'A')
                              {
                                *p++ = '0';
                                *p++ = dp->conversion - 'A' + 'X';
                                pad_ptr = p;
                                *p++ = '0';
                                if ((flags & FLAG_ALT) || precision > 0)
                                  {
                                    *p++ = decimal_point_char ();
                                    for (; precision > 0; precision--)
                                      *p++ = '0';
                                  }
                                *p++ = dp->conversion - 'A' + 'P';
                                *p++ = '+';
                                *p++ = '0';
                              }
                            else
                              abort ();
#  endif
                          }

                        END_LONG_DOUBLE_ROUNDING ();
                      }
                  }
#  if NEED_PRINTF_DOUBLE || NEED_PRINTF_INFINITE_DOUBLE
                else
#  endif
# endif
# if NEED_PRINTF_DOUBLE || NEED_PRINTF_INFINITE_DOUBLE
                  {
                    double arg = a.arg[dp->arg_index].a.a_double;

                    if (isnand (arg))
                      {
                        if (dp->conversion >= 'A' && dp->conversion <= 'Z')
                          {
                            *p++ = 'N'; *p++ = 'A'; *p++ = 'N';
                          }
                        else
                          {
                            *p++ = 'n'; *p++ = 'a'; *p++ = 'n';
                          }
                      }
                    else
                      {
                        int sign = 0;

                        if (signbit (arg))  
                          {
                            sign = -1;
                            arg = -arg;
                          }

                        if (sign < 0)
                          *p++ = '-';
                        else if (flags & FLAG_SHOWSIGN)
                          *p++ = '+';
                        else if (flags & FLAG_SPACE)
                          *p++ = ' ';

                        if (arg > 0.0 && arg + arg == arg)
                          {
                            if (dp->conversion >= 'A' && dp->conversion <= 'Z')
                              {
                                *p++ = 'I'; *p++ = 'N'; *p++ = 'F';
                              }
                            else
                              {
                                *p++ = 'i'; *p++ = 'n'; *p++ = 'f';
                              }
                          }
                        else
                          {
#  if NEED_PRINTF_DOUBLE
                            pad_ptr = p;

                            if (dp->conversion == 'f' || dp->conversion == 'F')
                              {
                                char *digits;
                                size_t ndigits;

                                digits =
                                  scale10_round_decimal_double (arg, precision);
                                if (digits == NULL)
                                  goto out_of_memory;
                                ndigits = strlen (digits);

                                if (ndigits > precision)
                                  do
                                    {
                                      --ndigits;
                                      *p++ = digits[ndigits];
                                    }
                                  while (ndigits > precision);
                                else
                                  *p++ = '0';
                                 
                                if ((flags & FLAG_ALT) || precision > 0)
                                  {
                                    *p++ = decimal_point_char ();
                                    for (; precision > ndigits; precision--)
                                      *p++ = '0';
                                    while (ndigits > 0)
                                      {
                                        --ndigits;
                                        *p++ = digits[ndigits];
                                      }
                                  }

                                free (digits);
                              }
                            else if (dp->conversion == 'e' || dp->conversion == 'E')
                              {
                                int exponent;

                                if (arg == 0.0)
                                  {
                                    exponent = 0;
                                    *p++ = '0';
                                    if ((flags & FLAG_ALT) || precision > 0)
                                      {
                                        *p++ = decimal_point_char ();
                                        for (; precision > 0; precision--)
                                          *p++ = '0';
                                      }
                                  }
                                else
                                  {
                                     
                                    int adjusted;
                                    char *digits;
                                    size_t ndigits;

                                    exponent = floorlog10 (arg);
                                    adjusted = 0;
                                    for (;;)
                                      {
                                        digits =
                                          scale10_round_decimal_double (arg,
                                                                        (int)precision - exponent);
                                        if (digits == NULL)
                                          goto out_of_memory;
                                        ndigits = strlen (digits);

                                        if (ndigits == precision + 1)
                                          break;
                                        if (ndigits < precision
                                            || ndigits > precision + 2)
                                           
                                          abort ();
                                        if (adjusted)
                                           
                                          abort ();
                                        free (digits);
                                        if (ndigits == precision)
                                          exponent -= 1;
                                        else
                                          exponent += 1;
                                        adjusted = 1;
                                      }
                                     
                                    if (is_borderline (digits, precision))
                                      {
                                         
                                        char *digits2 =
                                          scale10_round_decimal_double (arg,
                                                                        (int)precision - exponent + 1);
                                        if (digits2 == NULL)
                                          {
                                            free (digits);
                                            goto out_of_memory;
                                          }
                                        if (strlen (digits2) == precision + 1)
                                          {
                                            free (digits);
                                            digits = digits2;
                                            exponent -= 1;
                                          }
                                        else
                                          free (digits2);
                                      }
                                     

                                    *p++ = digits[--ndigits];
                                    if ((flags & FLAG_ALT) || precision > 0)
                                      {
                                        *p++ = decimal_point_char ();
                                        while (ndigits > 0)
                                          {
                                            --ndigits;
                                            *p++ = digits[ndigits];
                                          }
                                      }

                                    free (digits);
                                  }

                                *p++ = dp->conversion;  
#   if WIDE_CHAR_VERSION
                                {
                                  static const wchar_t decimal_format[] =
                                     
#    if defined _WIN32 && ! defined __CYGWIN__
                                    { '%', '+', '.', '3', 'd', '\0' };
#    else
                                    { '%', '+', '.', '2', 'd', '\0' };
#    endif
                                  SNPRINTF (p, 6 + 1, decimal_format, exponent);
                                }
                                while (*p != '\0')
                                  p++;
#   else
                                {
                                  static const char decimal_format[] =
                                     
#    if defined _WIN32 && ! defined __CYGWIN__
                                    "%+.3d";
#    else
                                    "%+.2d";
#    endif
                                  if (sizeof (DCHAR_T) == 1)
                                    {
                                      sprintf ((char *) p, decimal_format, exponent);
                                      while (*p != '\0')
                                        p++;
                                    }
                                  else
                                    {
                                      char expbuf[6 + 1];
                                      const char *ep;
                                      sprintf (expbuf, decimal_format, exponent);
                                      for (ep = expbuf; (*p = *ep) != '\0'; ep++)
                                        p++;
                                    }
                                }
#   endif
                              }
                            else if (dp->conversion == 'g' || dp->conversion == 'G')
                              {
                                if (precision == 0)
                                  precision = 1;
                                 

                                if (arg == 0.0)
                                   
                                  {
                                    size_t ndigits = precision;
                                     
                                    size_t nzeroes =
                                      (flags & FLAG_ALT ? 0 : precision - 1);

                                    --ndigits;
                                    *p++ = '0';
                                    if ((flags & FLAG_ALT) || ndigits > nzeroes)
                                      {
                                        *p++ = decimal_point_char ();
                                        while (ndigits > nzeroes)
                                          {
                                            --ndigits;
                                            *p++ = '0';
                                          }
                                      }
                                  }
                                else
                                  {
                                     
                                    int exponent;
                                    int adjusted;
                                    char *digits;
                                    size_t ndigits;
                                    size_t nzeroes;

                                    exponent = floorlog10 (arg);
                                    adjusted = 0;
                                    for (;;)
                                      {
                                        digits =
                                          scale10_round_decimal_double (arg,
                                                                        (int)(precision - 1) - exponent);
                                        if (digits == NULL)
                                          goto out_of_memory;
                                        ndigits = strlen (digits);

                                        if (ndigits == precision)
                                          break;
                                        if (ndigits < precision - 1
                                            || ndigits > precision + 1)
                                           
                                          abort ();
                                        if (adjusted)
                                           
                                          abort ();
                                        free (digits);
                                        if (ndigits < precision)
                                          exponent -= 1;
                                        else
                                          exponent += 1;
                                        adjusted = 1;
                                      }
                                     
                                    if (is_borderline (digits, precision - 1))
                                      {
                                         
                                        char *digits2 =
                                          scale10_round_decimal_double (arg,
                                                                        (int)(precision - 1) - exponent + 1);
                                        if (digits2 == NULL)
                                          {
                                            free (digits);
                                            goto out_of_memory;
                                          }
                                        if (strlen (digits2) == precision)
                                          {
                                            free (digits);
                                            digits = digits2;
                                            exponent -= 1;
                                          }
                                        else
                                          free (digits2);
                                      }
                                     

                                     
                                    nzeroes = 0;
                                    if ((flags & FLAG_ALT) == 0)
                                      while (nzeroes < ndigits
                                             && digits[nzeroes] == '0')
                                        nzeroes++;

                                     
                                    if (exponent >= -4
                                        && exponent < (long)precision)
                                      {
                                         
                                        if (exponent >= 0)
                                          {
                                            size_t ecount = exponent + 1;
                                             
                                            for (; ecount > 0; ecount--)
                                              *p++ = digits[--ndigits];
                                            if ((flags & FLAG_ALT) || ndigits > nzeroes)
                                              {
                                                *p++ = decimal_point_char ();
                                                while (ndigits > nzeroes)
                                                  {
                                                    --ndigits;
                                                    *p++ = digits[ndigits];
                                                  }
                                              }
                                          }
                                        else
                                          {
                                            size_t ecount = -exponent - 1;
                                            *p++ = '0';
                                            *p++ = decimal_point_char ();
                                            for (; ecount > 0; ecount--)
                                              *p++ = '0';
                                            while (ndigits > nzeroes)
                                              {
                                                --ndigits;
                                                *p++ = digits[ndigits];
                                              }
                                          }
                                      }
                                    else
                                      {
                                         
                                        *p++ = digits[--ndigits];
                                        if ((flags & FLAG_ALT) || ndigits > nzeroes)
                                          {
                                            *p++ = decimal_point_char ();
                                            while (ndigits > nzeroes)
                                              {
                                                --ndigits;
                                                *p++ = digits[ndigits];
                                              }
                                          }
                                        *p++ = dp->conversion - 'G' + 'E';  
#   if WIDE_CHAR_VERSION
                                        {
                                          static const wchar_t decimal_format[] =
                                             
#    if defined _WIN32 && ! defined __CYGWIN__
                                            { '%', '+', '.', '3', 'd', '\0' };
#    else
                                            { '%', '+', '.', '2', 'd', '\0' };
#    endif
                                          SNPRINTF (p, 6 + 1, decimal_format, exponent);
                                        }
                                        while (*p != '\0')
                                          p++;
#   else
                                        {
                                          static const char decimal_format[] =
                                             
#    if defined _WIN32 && ! defined __CYGWIN__
                                            "%+.3d";
#    else
                                            "%+.2d";
#    endif
                                          if (sizeof (DCHAR_T) == 1)
                                            {
                                              sprintf ((char *) p, decimal_format, exponent);
                                              while (*p != '\0')
                                                p++;
                                            }
                                          else
                                            {
                                              char expbuf[6 + 1];
                                              const char *ep;
                                              sprintf (expbuf, decimal_format, exponent);
                                              for (ep = expbuf; (*p = *ep) != '\0'; ep++)
                                                p++;
                                            }
                                        }
#   endif
                                      }

                                    free (digits);
                                  }
                              }
                            else
                              abort ();
#  else
                             
                            if (!(arg == 0.0))
                              abort ();

                            pad_ptr = p;

                            if (dp->conversion == 'f' || dp->conversion == 'F')
                              {
                                *p++ = '0';
                                if ((flags & FLAG_ALT) || precision > 0)
                                  {
                                    *p++ = decimal_point_char ();
                                    for (; precision > 0; precision--)
                                      *p++ = '0';
                                  }
                              }
                            else if (dp->conversion == 'e' || dp->conversion == 'E')
                              {
                                *p++ = '0';
                                if ((flags & FLAG_ALT) || precision > 0)
                                  {
                                    *p++ = decimal_point_char ();
                                    for (; precision > 0; precision--)
                                      *p++ = '0';
                                  }
                                *p++ = dp->conversion;  
                                *p++ = '+';
                                 
#   if defined _WIN32 && ! defined __CYGWIN__
                                *p++ = '0';
#   endif
                                *p++ = '0';
                                *p++ = '0';
                              }
                            else if (dp->conversion == 'g' || dp->conversion == 'G')
                              {
                                *p++ = '0';
                                if (flags & FLAG_ALT)
                                  {
                                    size_t ndigits =
                                      (precision > 0 ? precision - 1 : 0);
                                    *p++ = decimal_point_char ();
                                    for (; ndigits > 0; --ndigits)
                                      *p++ = '0';
                                  }
                              }
                            else
                              abort ();
#  endif
                          }
                      }
                  }
# endif

                 
                count = p - tmp;

                if (count < width)
                  {
                    size_t pad = width - count;
                    DCHAR_T *end = p + pad;

                    if (flags & FLAG_LEFT)
                      {
                         
                        for (; pad > 0; pad--)
                          *p++ = ' ';
                      }
                    else if ((flags & FLAG_ZERO) && pad_ptr != NULL)
                      {
                         
                        DCHAR_T *q = end;

                        while (p > pad_ptr)
                          *--q = *--p;
                        for (; pad > 0; pad--)
                          *p++ = '0';
                      }
                    else
                      {
                         
                        DCHAR_T *q = end;

                        while (p > tmp)
                          *--q = *--p;
                        for (; pad > 0; pad--)
                          *p++ = ' ';
                      }

                    p = end;
                  }

                count = p - tmp;

                if (count >= tmp_length)
                   
                  abort ();

                 
                if (count >= allocated - length)
                  {
                    size_t n = xsum (length, count);

                    ENSURE_ALLOCATION (n);
                  }

                 
                memcpy (result + length, tmp, count * sizeof (DCHAR_T));
                if (tmp != tmpbuf)
                  free (tmp);
                length += count;
              }
#endif
            else
              {
                arg_type type = a.arg[dp->arg_index].type;
                int flags = dp->flags;
#if !DCHAR_IS_TCHAR || ENABLE_UNISTDIO || NEED_PRINTF_FLAG_LEFTADJUST || NEED_PRINTF_FLAG_ZERO || NEED_PRINTF_UNBOUNDED_PRECISION
                int has_width;
#endif
#if !USE_SNPRINTF || !HAVE_SNPRINTF_RETVAL_C99 || USE_MSVC__SNPRINTF || !DCHAR_IS_TCHAR || ENABLE_UNISTDIO || NEED_PRINTF_FLAG_LEFTADJUST || NEED_PRINTF_FLAG_ZERO || NEED_PRINTF_UNBOUNDED_PRECISION
                size_t width;
#endif
#if !USE_SNPRINTF || !HAVE_SNPRINTF_RETVAL_C99 || USE_MSVC__SNPRINTF || NEED_PRINTF_UNBOUNDED_PRECISION
                int has_precision;
                size_t precision;
#endif
#if NEED_PRINTF_UNBOUNDED_PRECISION
                int prec_ourselves;
#else
#               define prec_ourselves 0
#endif
#if NEED_PRINTF_FLAG_LEFTADJUST
#               define pad_ourselves 1
#elif !DCHAR_IS_TCHAR || ENABLE_UNISTDIO || NEED_PRINTF_FLAG_ZERO || NEED_PRINTF_UNBOUNDED_PRECISION
                int pad_ourselves;
#else
#               define pad_ourselves 0
#endif
                TCHAR_T *fbp;
                unsigned int prefix_count;
                int prefixes[2] IF_LINT (= { 0 });
                int orig_errno;
#if !USE_SNPRINTF
                size_t tmp_length;
                TCHAR_T tmpbuf[700];
                TCHAR_T *tmp;
#endif

#if !DCHAR_IS_TCHAR || ENABLE_UNISTDIO || NEED_PRINTF_FLAG_LEFTADJUST || NEED_PRINTF_FLAG_ZERO || NEED_PRINTF_UNBOUNDED_PRECISION
                has_width = 0;
#endif
#if !USE_SNPRINTF || !HAVE_SNPRINTF_RETVAL_C99 || USE_MSVC__SNPRINTF || !DCHAR_IS_TCHAR || ENABLE_UNISTDIO || NEED_PRINTF_FLAG_LEFTADJUST || NEED_PRINTF_FLAG_ZERO || NEED_PRINTF_UNBOUNDED_PRECISION
                width = 0;
                if (dp->width_start != dp->width_end)
                  {
                    if (dp->width_arg_index != ARG_NONE)
                      {
                        int arg;

                        if (!(a.arg[dp->width_arg_index].type == TYPE_INT))
                          abort ();
                        arg = a.arg[dp->width_arg_index].a.a_int;
                        width = arg;
                        if (arg < 0)
                          {
                             
                            flags |= FLAG_LEFT;
                            width = -width;
                          }
                      }
                    else
                      {
                        const FCHAR_T *digitp = dp->width_start;

                        do
                          width = xsum (xtimes (width, 10), *digitp++ - '0');
                        while (digitp != dp->width_end);
                      }
#if !DCHAR_IS_TCHAR || ENABLE_UNISTDIO || NEED_PRINTF_FLAG_LEFTADJUST || NEED_PRINTF_FLAG_ZERO || NEED_PRINTF_UNBOUNDED_PRECISION
                    has_width = 1;
#endif
                  }
#endif

#if !USE_SNPRINTF || !HAVE_SNPRINTF_RETVAL_C99 || USE_MSVC__SNPRINTF || NEED_PRINTF_UNBOUNDED_PRECISION
                has_precision = 0;
                precision = 6;
                if (dp->precision_start != dp->precision_end)
                  {
                    if (dp->precision_arg_index != ARG_NONE)
                      {
                        int arg;

                        if (!(a.arg[dp->precision_arg_index].type == TYPE_INT))
                          abort ();
                        arg = a.arg[dp->precision_arg_index].a.a_int;
                         
                        if (arg >= 0)
                          {
                            precision = arg;
                            has_precision = 1;
                          }
                      }
                    else
                      {
                        const FCHAR_T *digitp = dp->precision_start + 1;

                        precision = 0;
                        while (digitp != dp->precision_end)
                          precision = xsum (xtimes (precision, 10), *digitp++ - '0');
                        has_precision = 1;
                      }
                  }
#endif

                 
#if NEED_PRINTF_UNBOUNDED_PRECISION
                switch (dp->conversion)
                  {
                  case 'd': case 'i': case 'u':
                  case 'o':
                  case 'x': case 'X': case 'p':
                    prec_ourselves = has_precision && (precision > 0);
                    break;
                  default:
                    prec_ourselves = 0;
                    break;
                  }
#endif

                 
#if !NEED_PRINTF_FLAG_LEFTADJUST && (!DCHAR_IS_TCHAR || ENABLE_UNISTDIO || NEED_PRINTF_FLAG_ZERO || NEED_PRINTF_UNBOUNDED_PRECISION)
                switch (dp->conversion)
                  {
# if !DCHAR_IS_TCHAR || ENABLE_UNISTDIO
                   
                  case 'c': case 's':
# endif
# if NEED_PRINTF_FLAG_ZERO
                  case 'f': case 'F': case 'e': case 'E': case 'g': case 'G':
                  case 'a': case 'A':
# endif
                    pad_ourselves = 1;
                    break;
                  default:
                    pad_ourselves = prec_ourselves;
                    break;
                  }
#endif

#if !USE_SNPRINTF
                 
                tmp_length =
                  MAX_ROOM_NEEDED (&a, dp->arg_index, dp->conversion, type,
                                   flags, width, has_precision, precision,
                                   pad_ourselves);

                if (tmp_length <= sizeof (tmpbuf) / sizeof (TCHAR_T))
                  tmp = tmpbuf;
                else
                  {
                    size_t tmp_memsize = xtimes (tmp_length, sizeof (TCHAR_T));

                    if (size_overflow_p (tmp_memsize))
                       
                      goto out_of_memory;
                    tmp = (TCHAR_T *) malloc (tmp_memsize);
                    if (tmp == NULL)
                       
                      goto out_of_memory;
                  }
#endif

                 
                fbp = buf;
                *fbp++ = '%';
#if NEED_PRINTF_FLAG_GROUPING
                 
#else
                if (flags & FLAG_GROUP)
                  *fbp++ = '\'';
#endif
                if (flags & FLAG_LEFT)
                  *fbp++ = '-';
                if (flags & FLAG_SHOWSIGN)
                  *fbp++ = '+';
                if (flags & FLAG_SPACE)
                  *fbp++ = ' ';
                if (flags & FLAG_ALT)
                  *fbp++ = '#';
#if __GLIBC__ >= 2 && !defined __UCLIBC__
                if (flags & FLAG_LOCALIZED)
                  *fbp++ = 'I';
#endif
                if (!pad_ourselves)
                  {
                    if (flags & FLAG_ZERO)
                      *fbp++ = '0';
                    if (dp->width_start != dp->width_end)
                      {
                        size_t n = dp->width_end - dp->width_start;
                         
                        if (sizeof (FCHAR_T) == sizeof (TCHAR_T))
                          {
                            memcpy (fbp, dp->width_start, n * sizeof (TCHAR_T));
                            fbp += n;
                          }
                        else
                          {
                            const FCHAR_T *mp = dp->width_start;
                            do
                              *fbp++ = *mp++;
                            while (--n > 0);
                          }
                      }
                  }
                if (!prec_ourselves)
                  {
                    if (dp->precision_start != dp->precision_end)
                      {
                        size_t n = dp->precision_end - dp->precision_start;
                         
                        if (sizeof (FCHAR_T) == sizeof (TCHAR_T))
                          {
                            memcpy (fbp, dp->precision_start, n * sizeof (TCHAR_T));
                            fbp += n;
                          }
                        else
                          {
                            const FCHAR_T *mp = dp->precision_start;
                            do
                              *fbp++ = *mp++;
                            while (--n > 0);
                          }
                      }
                  }

                switch (type)
                  {
#if HAVE_LONG_LONG_INT
                  case TYPE_LONGLONGINT:
                  case TYPE_ULONGLONGINT:
# if defined _WIN32 && ! defined __CYGWIN__
                    *fbp++ = 'I';
                    *fbp++ = '6';
                    *fbp++ = '4';
                    break;
# else
                    *fbp++ = 'l';
# endif
#endif
                    FALLTHROUGH;
                  case TYPE_LONGINT:
                  case TYPE_ULONGINT:
#if HAVE_WINT_T
                  case TYPE_WIDE_CHAR:
#endif
#if HAVE_WCHAR_T
                  case TYPE_WIDE_STRING:
#endif
                    *fbp++ = 'l';
                    break;
                  case TYPE_LONGDOUBLE:
                    *fbp++ = 'L';
                    break;
                  default:
                    break;
                  }
#if NEED_PRINTF_DIRECTIVE_F
                if (dp->conversion == 'F')
                  *fbp = 'f';
                else
#endif
                  *fbp = dp->conversion;
#if USE_SNPRINTF
# if ! (((__GLIBC__ > 2 || (__GLIBC__ == 2 && __GLIBC_MINOR__ >= 3))        \
         && !defined __UCLIBC__)                                            \
        || (defined __APPLE__ && defined __MACH__)                          \
        || (defined _WIN32 && ! defined __CYGWIN__))
                fbp[1] = '%';
                fbp[2] = 'n';
                fbp[3] = '\0';
# else
                 
                 
                 
                fbp[1] = '\0';
# endif
#else
                fbp[1] = '\0';
#endif

                 
                prefix_count = 0;
                if (!pad_ourselves && dp->width_arg_index != ARG_NONE)
                  {
                    if (!(a.arg[dp->width_arg_index].type == TYPE_INT))
                      abort ();
                    prefixes[prefix_count++] = a.arg[dp->width_arg_index].a.a_int;
                  }
                if (!prec_ourselves && dp->precision_arg_index != ARG_NONE)
                  {
                    if (!(a.arg[dp->precision_arg_index].type == TYPE_INT))
                      abort ();
                    prefixes[prefix_count++] = a.arg[dp->precision_arg_index].a.a_int;
                  }

#if USE_SNPRINTF
                 
# define TCHARS_PER_DCHAR (sizeof (DCHAR_T) / sizeof (TCHAR_T))
                 
                ENSURE_ALLOCATION (xsum (length,
                                         (2 + TCHARS_PER_DCHAR - 1)
                                         / TCHARS_PER_DCHAR));
                 
                *(TCHAR_T *) (result + length) = '\0';
#endif

                orig_errno = errno;

                for (;;)
                  {
                    int count = -1;

#if USE_SNPRINTF
                    int retcount = 0;
                    size_t maxlen = allocated - length;
                     
                    if (maxlen > INT_MAX / TCHARS_PER_DCHAR)
                      maxlen = INT_MAX / TCHARS_PER_DCHAR;
                    maxlen = maxlen * TCHARS_PER_DCHAR;
# define SNPRINTF_BUF(arg) \
                    switch (prefix_count)                                   \
                      {                                                     \
                      case 0:                                               \
                        retcount = SNPRINTF ((TCHAR_T *) (result + length), \
                                             maxlen, buf,                   \
                                             arg, &count);                  \
                        break;                                              \
                      case 1:                                               \
                        retcount = SNPRINTF ((TCHAR_T *) (result + length), \
                                             maxlen, buf,                   \
                                             prefixes[0], arg, &count);     \
                        break;                                              \
                      case 2:                                               \
                        retcount = SNPRINTF ((TCHAR_T *) (result + length), \
                                             maxlen, buf,                   \
                                             prefixes[0], prefixes[1], arg, \
                                             &count);                       \
                        break;                                              \
                      default:                                              \
                        abort ();                                           \
                      }
#else
# define SNPRINTF_BUF(arg) \
                    switch (prefix_count)                                   \
                      {                                                     \
                      case 0:                                               \
                        count = sprintf (tmp, buf, arg);                    \
                        break;                                              \
                      case 1:                                               \
                        count = sprintf (tmp, buf, prefixes[0], arg);       \
                        break;                                              \
                      case 2:                                               \
                        count = sprintf (tmp, buf, prefixes[0], prefixes[1],\
                                         arg);                              \
                        break;                                              \
                      default:                                              \
                        abort ();                                           \
                      }
#endif

                    errno = 0;
                    switch (type)
                      {
                      case TYPE_SCHAR:
                        {
                          int arg = a.arg[dp->arg_index].a.a_schar;
                          SNPRINTF_BUF (arg);
                        }
                        break;
                      case TYPE_UCHAR:
                        {
                          unsigned int arg = a.arg[dp->arg_index].a.a_uchar;
                          SNPRINTF_BUF (arg);
                        }
                        break;
                      case TYPE_SHORT:
                        {
                          int arg = a.arg[dp->arg_index].a.a_short;
                          SNPRINTF_BUF (arg);
                        }
                        break;
                      case TYPE_USHORT:
                        {
                          unsigned int arg = a.arg[dp->arg_index].a.a_ushort;
                          SNPRINTF_BUF (arg);
                        }
                        break;
                      case TYPE_INT:
                        {
                          int arg = a.arg[dp->arg_index].a.a_int;
                          SNPRINTF_BUF (arg);
                        }
                        break;
                      case TYPE_UINT:
                        {
                          unsigned int arg = a.arg[dp->arg_index].a.a_uint;
                          SNPRINTF_BUF (arg);
                        }
                        break;
                      case TYPE_LONGINT:
                        {
                          long int arg = a.arg[dp->arg_index].a.a_longint;
                          SNPRINTF_BUF (arg);
                        }
                        break;
                      case TYPE_ULONGINT:
                        {
                          unsigned long int arg = a.arg[dp->arg_index].a.a_ulongint;
                          SNPRINTF_BUF (arg);
                        }
                        break;
#if HAVE_LONG_LONG_INT
                      case TYPE_LONGLONGINT:
                        {
                          long long int arg = a.arg[dp->arg_index].a.a_longlongint;
                          SNPRINTF_BUF (arg);
                        }
                        break;
                      case TYPE_ULONGLONGINT:
                        {
                          unsigned long long int arg = a.arg[dp->arg_index].a.a_ulonglongint;
                          SNPRINTF_BUF (arg);
                        }
                        break;
#endif
                      case TYPE_DOUBLE:
                        {
                          double arg = a.arg[dp->arg_index].a.a_double;
                          SNPRINTF_BUF (arg);
                        }
                        break;
                      case TYPE_LONGDOUBLE:
                        {
                          long double arg = a.arg[dp->arg_index].a.a_longdouble;
                          SNPRINTF_BUF (arg);
                        }
                        break;
                      case TYPE_CHAR:
                        {
                          int arg = a.arg[dp->arg_index].a.a_char;
                          SNPRINTF_BUF (arg);
                        }
                        break;
#if HAVE_WINT_T
                      case TYPE_WIDE_CHAR:
                        {
                          wint_t arg = a.arg[dp->arg_index].a.a_wide_char;
                          SNPRINTF_BUF (arg);
                        }
                        break;
#endif
                      case TYPE_STRING:
                        {
                          const char *arg = a.arg[dp->arg_index].a.a_string;
                          SNPRINTF_BUF (arg);
                        }
                        break;
#if HAVE_WCHAR_T
                      case TYPE_WIDE_STRING:
                        {
                          const wchar_t *arg = a.arg[dp->arg_index].a.a_wide_string;
                          SNPRINTF_BUF (arg);
                        }
                        break;
#endif
                      case TYPE_POINTER:
                        {
                          void *arg = a.arg[dp->arg_index].a.a_pointer;
                          SNPRINTF_BUF (arg);
                        }
                        break;
                      default:
                        abort ();
                      }

#if USE_SNPRINTF
                     
                    if (count >= 0)
                      {
                         
                        if (count < maxlen
                            && ((TCHAR_T *) (result + length)) [count] != '\0')
                          abort ();
                         
                        if (retcount > count)
                          count = retcount;
                      }
                    else
                      {
                         
                        if (fbp[1] != '\0')
                          {
                             
                            fbp[1] = '\0';
                            continue;
                          }
                        else
                          {
                             
                            if (retcount < 0)
                              {
# if !HAVE_SNPRINTF_RETVAL_C99 || USE_MSVC__SNPRINTF
                                 
                                size_t tmp_length =
                                  MAX_ROOM_NEEDED (&a, dp->arg_index,
                                                   dp->conversion, type, flags,
                                                   width,
                                                   has_precision,
                                                   precision, pad_ourselves);

                                if (maxlen < tmp_length)
                                  {
                                     
                                    size_t bigger_need =
                                      xsum (length,
                                            xsum (tmp_length,
                                                  TCHARS_PER_DCHAR - 1)
                                            / TCHARS_PER_DCHAR);
                                     
                                    size_t bigger_need2 =
                                      xsum (xtimes (allocated, 2), 12);
                                    if (bigger_need < bigger_need2)
                                      bigger_need = bigger_need2;
                                    ENSURE_ALLOCATION (bigger_need);
                                    continue;
                                  }
# endif
                              }
                            else
                              count = retcount;
                          }
                      }
#endif

                     
                    if (count < 0)
                      {
                         
                        int saved_errno = errno;
                        if (saved_errno == 0)
                          {
                            if (dp->conversion == 'c' || dp->conversion == 's')
                              saved_errno = EILSEQ;
                            else
                              saved_errno = EINVAL;
                          }

                        if (!(result == resultbuf || result == NULL))
                          free (result);
                        if (buf_malloced != NULL)
                          free (buf_malloced);
                        CLEANUP ();

                        errno = saved_errno;
                        return NULL;
                      }

#if USE_SNPRINTF
                     
                    if ((unsigned int) count + 1 >= maxlen)
                      {
                         
                        if (maxlen == INT_MAX / TCHARS_PER_DCHAR)
                          goto overflow;
                        else
                          {
                             
                            size_t n =
                              xmax (xsum (length,
                                          ((unsigned int) count + 2
                                           + TCHARS_PER_DCHAR - 1)
                                          / TCHARS_PER_DCHAR),
                                    xtimes (allocated, 2));

                            ENSURE_ALLOCATION (n);
                            continue;
                          }
                      }
#endif

#if NEED_PRINTF_UNBOUNDED_PRECISION
                    if (prec_ourselves)
                      {
                         
                        TCHAR_T *prec_ptr =
# if USE_SNPRINTF
                          (TCHAR_T *) (result + length);
# else
                          tmp;
# endif
                        size_t prefix_count;
                        size_t move;

                        prefix_count = 0;
                         
                        if (count >= 1
                            && (*prec_ptr == '-' || *prec_ptr == '+'
                                || *prec_ptr == ' '))
                          prefix_count = 1;
                         
                        else if (count >= 2
                                 && prec_ptr[0] == '0'
                                 && (prec_ptr[1] == 'x' || prec_ptr[1] == 'X'))
                          prefix_count = 2;

                        move = count - prefix_count;
                        if (precision > move)
                          {
                             
                            size_t insert = precision - move;
                            TCHAR_T *prec_end;

# if USE_SNPRINTF
                            size_t n =
                              xsum (length,
                                    (count + insert + TCHARS_PER_DCHAR - 1)
                                    / TCHARS_PER_DCHAR);
                            length += (count + TCHARS_PER_DCHAR - 1) / TCHARS_PER_DCHAR;
                            ENSURE_ALLOCATION (n);
                            length -= (count + TCHARS_PER_DCHAR - 1) / TCHARS_PER_DCHAR;
                            prec_ptr = (TCHAR_T *) (result + length);
# endif

                            prec_end = prec_ptr + count;
                            prec_ptr += prefix_count;

                            while (prec_end > prec_ptr)
                              {
                                prec_end--;
                                prec_end[insert] = prec_end[0];
                              }

                            prec_end += insert;
                            do
                              *--prec_end = '0';
                            while (prec_end > prec_ptr);

                            count += insert;
                          }
                      }
#endif

#if !USE_SNPRINTF
                    if (count >= tmp_length)
                       
                      abort ();
#endif

#if !DCHAR_IS_TCHAR
                     
                    if (dp->conversion == 'c' || dp->conversion == 's')
                      {
                         
                        const TCHAR_T *tmpsrc;
                        DCHAR_T *tmpdst;
                        size_t tmpdst_len;
                         
                        verify (sizeof (TCHAR_T) == 1);
# if USE_SNPRINTF
                        tmpsrc = (TCHAR_T *) (result + length);
# else
                        tmpsrc = tmp;
# endif
                        tmpdst =
                          DCHAR_CONV_FROM_ENCODING (locale_charset (),
                                                    iconveh_question_mark,
                                                    tmpsrc, count,
                                                    NULL,
                                                    NULL, &tmpdst_len);
                        if (tmpdst == NULL)
                          {
                            int saved_errno = errno;
                            if (!(result == resultbuf || result == NULL))
                              free (result);
                            if (buf_malloced != NULL)
                              free (buf_malloced);
                            CLEANUP ();
                            errno = saved_errno;
                            return NULL;
                          }
                        ENSURE_ALLOCATION (xsum (length, tmpdst_len));
                        DCHAR_CPY (result + length, tmpdst, tmpdst_len);
                        free (tmpdst);
                        count = tmpdst_len;
                      }
                    else
                      {
                         
# if USE_SNPRINTF
                         
                        if (sizeof (DCHAR_T) != sizeof (TCHAR_T))
# endif
                          {
                            const TCHAR_T *tmpsrc;
                            DCHAR_T *tmpdst;
                            size_t n;

# if USE_SNPRINTF
                            if (result == resultbuf)
                              {
                                tmpsrc = (TCHAR_T *) (result + length);
                                 
                                ENSURE_ALLOCATION (xsum (length, count));
                              }
                            else
                              {
                                 
                                ENSURE_ALLOCATION (xsum (length, count));
                                tmpsrc = (TCHAR_T *) (result + length);
                              }
# else
                            tmpsrc = tmp;
                            ENSURE_ALLOCATION (xsum (length, count));
# endif
                            tmpdst = result + length;
                             
                            tmpsrc += count;
                            tmpdst += count;
                            for (n = count; n > 0; n--)
                              *--tmpdst = *--tmpsrc;
                          }
                      }
#endif

#if DCHAR_IS_TCHAR && !USE_SNPRINTF
                     
                    if (count > allocated - length)
                      {
                         
                        size_t n =
                          xmax (xsum (length, count), xtimes (allocated, 2));

                        ENSURE_ALLOCATION (n);
                      }
#endif

                     

                     
#if !DCHAR_IS_TCHAR || ENABLE_UNISTDIO || NEED_PRINTF_FLAG_LEFTADJUST || NEED_PRINTF_FLAG_ZERO || NEED_PRINTF_UNBOUNDED_PRECISION
                    if (pad_ourselves && has_width)
                      {
                        size_t w;
# if ENABLE_UNISTDIO
                         
                        w = DCHAR_MBSNLEN (result + length, count);
# else
                         
                        w = count;
# endif
                        if (w < width)
                          {
                            size_t pad = width - w;

                             
                            if (xsum (count, pad) > allocated - length)
                              {
                                 
                                size_t n =
                                  xmax (xsum3 (length, count, pad),
                                        xtimes (allocated, 2));

# if USE_SNPRINTF
                                length += count;
                                ENSURE_ALLOCATION (n);
                                length -= count;
# else
                                ENSURE_ALLOCATION (n);
# endif
                              }
                             

                            {
# if !DCHAR_IS_TCHAR || USE_SNPRINTF
                              DCHAR_T * const rp = result + length;
# else
                              DCHAR_T * const rp = tmp;
# endif
                              DCHAR_T *p = rp + count;
                              DCHAR_T *end = p + pad;
                              DCHAR_T *pad_ptr;
# if !DCHAR_IS_TCHAR || ENABLE_UNISTDIO
                              if (dp->conversion == 'c'
                                  || dp->conversion == 's')
                                 
                                pad_ptr = NULL;
                              else
# endif
                                {
                                  pad_ptr = (*rp == '-' ? rp + 1 : rp);
                                   
                                  if ((*pad_ptr >= 'A' && *pad_ptr <= 'Z')
                                      || (*pad_ptr >= 'a' && *pad_ptr <= 'z'))
                                    pad_ptr = NULL;
                                }
                               

                              count = count + pad;  

                              if (flags & FLAG_LEFT)
                                {
                                   
                                  for (; pad > 0; pad--)
                                    *p++ = ' ';
                                }
                              else if ((flags & FLAG_ZERO) && pad_ptr != NULL)
                                {
                                   
                                  DCHAR_T *q = end;

                                  while (p > pad_ptr)
                                    *--q = *--p;
                                  for (; pad > 0; pad--)
                                    *p++ = '0';
                                }
                              else
                                {
                                   
                                  DCHAR_T *q = end;

                                  while (p > rp)
                                    *--q = *--p;
                                  for (; pad > 0; pad--)
                                    *p++ = ' ';
                                }
                            }
                          }
                      }
#endif

                     

#if !DCHAR_IS_TCHAR || USE_SNPRINTF
                     
#else
                     
                    memcpy (result + length, tmp, count * sizeof (DCHAR_T));
#endif
#if !USE_SNPRINTF
                    if (tmp != tmpbuf)
                      free (tmp);
#endif

#if NEED_PRINTF_DIRECTIVE_F
                    if (dp->conversion == 'F')
                      {
                         
                        DCHAR_T *rp = result + length;
                        size_t rc;
                        for (rc = count; rc > 0; rc--, rp++)
                          if (*rp >= 'a' && *rp <= 'z')
                            *rp = *rp - 'a' + 'A';
                      }
#endif

                    length += count;
                    break;
                  }
                errno = orig_errno;
#undef pad_ourselves
#undef prec_ourselves
              }
          }
      }

     
    ENSURE_ALLOCATION (xsum (length, 1));
    result[length] = '\0';

    if (result != resultbuf && length + 1 < allocated)
      {
         
        DCHAR_T *memory;

        memory = (DCHAR_T *) realloc (result, (length + 1) * sizeof (DCHAR_T));
        if (memory != NULL)
          result = memory;
      }

    if (buf_malloced != NULL)
      free (buf_malloced);
    CLEANUP ();
    *lengthp = length;
     
    return result;

#if USE_SNPRINTF
  overflow:
    if (!(result == resultbuf || result == NULL))
      free (result);
    if (buf_malloced != NULL)
      free (buf_malloced);
    CLEANUP ();
    errno = EOVERFLOW;
    return NULL;
#endif

  out_of_memory:
    if (!(result == resultbuf || result == NULL))
      free (result);
    if (buf_malloced != NULL)
      free (buf_malloced);
  out_of_memory_1:
    CLEANUP ();
    errno = ENOMEM;
    return NULL;
  }
}
