static void generate_seqset(struct Buffer *b, struct ImapData *idata,
                            unsigned int msn_begin, unsigned int msn_end)
{
  int chunks = 0;
  int state = 0;  
  unsigned int msn, range_begin, range_end;

  for (msn = msn_begin; msn <= msn_end + 1; msn++)
  {
    if (msn <= msn_end && !idata->msn_index[msn - 1])
    {
      switch (state)
      {
        case 1:  
          state = 2;
         
        case 2:  
          range_end = msn;
          break;
        default:
          state = 1;
          range_begin = msn;
          break;
      }
    }
    else if (state)
    {
      if (chunks++)
        mutt_buffer_addch(b, ',');
      if (chunks == 150)
        break;

      if (state == 1)
        mutt_buffer_printf(b, "%u", range_begin);
      else if (state == 2)
        mutt_buffer_printf(b, "%u:%u", range_begin, range_end);
      state = 0;
    }
  }

   
  if (chunks == 150 || mutt_str_strlen(b->data) > 500)
  {
    b->dptr = b->data;
    mutt_buffer_printf(b, "%u:%u", msn_begin, msn_end);
  }
}
