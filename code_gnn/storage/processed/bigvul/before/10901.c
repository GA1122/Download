ksba_ocsp_get_sig_val (ksba_ocsp_t ocsp, ksba_isotime_t produced_at)
{
  ksba_sexp_t p;

  if (produced_at)
    *produced_at = 0;
  if (!ocsp || !ocsp->sigval )
    return NULL;

  if (produced_at)
    _ksba_copy_time (produced_at, ocsp->produced_at);

  p = ocsp->sigval;
  ocsp->sigval = NULL;
  return p;
}
