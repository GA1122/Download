ecc_get_nbits (gcry_sexp_t parms)
{
  gcry_sexp_t l1;
  gcry_mpi_t p;
  unsigned int nbits = 0;
  char *curve;

  l1 = sexp_find_token (parms, "p", 1);
  if (!l1)
    {  
      l1 = sexp_find_token (parms, "curve", 5);
      if (!l1)
        return 0;  

      curve = sexp_nth_string (l1, 1);
      sexp_release (l1);
      if (!curve)
        return 0;   

      if (_gcry_ecc_fill_in_curve (0, curve, NULL, &nbits))
        nbits = 0;
      xfree (curve);
    }
  else
    {
      p = sexp_nth_mpi (l1, 1, GCRYMPI_FMT_USG);
      sexp_release (l1);
      if (p)
        {
          nbits = mpi_get_nbits (p);
          _gcry_mpi_release (p);
        }
    }
  return nbits;
}
