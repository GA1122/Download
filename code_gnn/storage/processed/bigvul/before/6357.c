import_keys (ctrl_t ctrl, char **fnames, int nnames,
	     void *stats_handle, unsigned int options )
{
  import_keys_internal (ctrl, NULL, fnames, nnames, stats_handle,
                        NULL, NULL, options);
}
