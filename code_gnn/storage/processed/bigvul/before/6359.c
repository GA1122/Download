import_keys_stream (ctrl_t ctrl, IOBUF inp, void *stats_handle,
		    unsigned char **fpr, size_t *fpr_len,unsigned int options)
{
  return import_keys_internal (ctrl, inp, NULL, 0, stats_handle,
                               fpr, fpr_len, options);
}
