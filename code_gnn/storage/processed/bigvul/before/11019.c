dump_decoder_state (DECODER_STATE ds)
{
  int i;

  for (i=0; i < ds->idx; i++)
    {
      fprintf (stderr,"  ds stack[%d] (", i);
      if (ds->stack[i].node)
        _ksba_asn_node_dump (ds->stack[i].node, stderr);
      else
        fprintf (stderr, "Null");
      fprintf (stderr,") %s%d (%d)%s\n",
               ds->stack[i].ndef_length? "ndef ":"",
               ds->stack[i].length,
               ds->stack[i].nread,
               ds->stack[i].in_seq_of? " in_seq_of":"");
    }
 }
