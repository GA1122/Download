new_decoder_state (void)
{
  DECODER_STATE ds;

  ds = xmalloc (sizeof (*ds) + 99*sizeof(DECODER_STATE_ITEM));
  ds->stacksize = 100;
  ds->idx = 0;
  ds->cur.node = NULL;
  ds->cur.went_up = 0;
  ds->cur.in_seq_of = 0;
  ds->cur.in_any = 0;
  ds->cur.again = 0;
  ds->cur.next_tag = 0;
  ds->cur.length = 0;
  ds->cur.ndef_length = 1;
  ds->cur.nread = 0;
  return ds;
}
