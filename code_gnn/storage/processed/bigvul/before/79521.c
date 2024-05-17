void mutt_rfc2047_decode(char **pd)
{
  if (!pd || !*pd)
    return;

  struct Buffer buf = { 0 };  
  char *s = *pd;              
  char *beg;                  
  enum ContentEncoding enc;   
  char *charset;              
  size_t charsetlen;          
  char *text;                 
  size_t textlen;             

   
  struct Buffer prev = { 0 };  
  char *prev_charset = NULL;   
  size_t prev_charsetlen = 0;  

  while (*s)
  {
    beg = parse_encoded_word(s, &enc, &charset, &charsetlen, &text, &textlen);
    if (beg != s)
    {
       
      size_t holelen = beg ? beg - s : mutt_str_strlen(s);

       
      if (beg && mutt_str_lws_len(s, holelen) == holelen)
      {
        s = beg;
        continue;
      }

       
      if (prev.data)
      {
        finalize_chunk(&buf, &prev, prev_charset, prev_charsetlen);
      }

       
      {
        if (AssumedCharset && *AssumedCharset)
        {
          char *conv = mutt_str_substr_dup(s, s + holelen);
          mutt_ch_convert_nonmime_string(&conv);
          mutt_buffer_addstr(&buf, conv);
          FREE(&conv);
        }
        else
        {
          mutt_buffer_add(&buf, s, holelen);
        }
      }
      s += holelen;
    }
    if (beg)
    {
       
      text[textlen] = '\0';
      char *decoded = rfc2047_decode_word(text, textlen, enc);
      if (decoded == NULL)
      {
        return;
      }
      if (prev.data && ((prev_charsetlen != charsetlen) ||
                        (strncmp(prev_charset, charset, charsetlen) != 0)))
      {
         
        finalize_chunk(&buf, &prev, prev_charset, prev_charsetlen);
      }

      mutt_buffer_addstr(&prev, decoded);
      FREE(&decoded);
      prev_charset = charset;
      prev_charsetlen = charsetlen;
      s = text + textlen + 2;  
    }
  }

   
  if (prev.data)
  {
    finalize_chunk(&buf, &prev, prev_charset, prev_charsetlen);
  }

  mutt_buffer_addch(&buf, '\0');
  *pd = buf.data;
}
