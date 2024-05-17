static void load_creator_from_old_format(
    FILE       *fp,
    xref_t     *xref,
    const char *buf)
{
    int            i, n_eles, length, is_escaped, obj_id;
    char          *c, *ascii, *start, *s, *saved_buf_search, *obj;
    pdf_creator_t *info;

    info = new_creator(&n_eles);

    for (i=0; i<n_eles; ++i)
    {
        if (!(c = strstr(buf, info[i].key)))
          continue;

         
        c += strlen(info[i].key);
        while (isspace(*c))
          ++c;

         
        if (*c == '/')
          continue;

         
        obj = saved_buf_search = NULL;
        if (isdigit(*c))
        {
            obj_id = atoi(c);
            saved_buf_search = c;
            s = saved_buf_search;

            obj = get_object(fp, obj_id, xref, NULL, NULL);
            c = obj;

             
            while (c && (*c != '('))
             ++c;

             
            while (s && (*s == '/'))
              ++s;
            saved_buf_search = s;
        }
          
         
        start = c;
        length = is_escaped = 0;
        while (c && ((*c != '\r') && (*c != '\n') && (*c != '<')))
        {
             
            if (!is_escaped && (*c == ')'))
              break;
            else if (*c == '\\')
              is_escaped = 1;
            else
              is_escaped = 0;

            ++c;
            ++length;
        }

        if (length == 0)
          continue;

         
        if (length)
          length += 1;
        length = (length > KV_MAX_VALUE_LENGTH) ? KV_MAX_VALUE_LENGTH : length;
        strncpy(info[i].value, start, length);
        info[i].value[KV_MAX_VALUE_LENGTH - 1] = '\0';

         
        if (saved_buf_search)
        {
             
            free(obj);
            c = saved_buf_search;
        }
    }  

     
    for (i=0; i<n_eles; ++i)
      if ((ascii = decode_text_string(info[i].value, strlen(info[i].value))))
      {
          strncpy(info[i].value, ascii, strlen(info[i].value));
          free(ascii);
      }

    xref->creator = info;
    xref->n_creator_entries = n_eles;
}
