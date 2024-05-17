import_keys_es_stream (ctrl_t ctrl, estream_t fp, void *stats_handle,
                       unsigned char **fpr, size_t *fpr_len,
                       unsigned int options)
{
  int rc;
  iobuf_t inp;

  inp = iobuf_esopen (fp, "r", 1);
  if (!inp)
    {
      rc = gpg_error_from_syserror ();
      log_error ("iobuf_esopen failed: %s\n", gpg_strerror (rc));
      return rc;
    }

  rc = import_keys_internal (ctrl, inp, NULL, 0, stats_handle,
                             fpr, fpr_len, options);

  iobuf_close (inp);
  return rc;
}
