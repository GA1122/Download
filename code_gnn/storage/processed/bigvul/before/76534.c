decode_long_double (long double x, int *ep, mpn_t *mp)
{
  mpn_t m;
  int exp;
  long double y;
  size_t i;

   
  m.nlimbs = (LDBL_MANT_BIT + GMP_LIMB_BITS - 1) / GMP_LIMB_BITS;
  m.limbs = (mp_limb_t *) malloc (m.nlimbs * sizeof (mp_limb_t));
  if (m.limbs == NULL)
    return NULL;
   
  y = frexpl (x, &exp);
  if (!(y >= 0.0L && y < 1.0L))
    abort ();
   
   
#  if (LDBL_MANT_BIT % GMP_LIMB_BITS) != 0
#   if (LDBL_MANT_BIT % GMP_LIMB_BITS) > GMP_LIMB_BITS / 2
    {
      mp_limb_t hi, lo;
      y *= (mp_limb_t) 1 << (LDBL_MANT_BIT % (GMP_LIMB_BITS / 2));
      hi = (int) y;
      y -= hi;
      if (!(y >= 0.0L && y < 1.0L))
        abort ();
      y *= (mp_limb_t) 1 << (GMP_LIMB_BITS / 2);
      lo = (int) y;
      y -= lo;
      if (!(y >= 0.0L && y < 1.0L))
        abort ();
      m.limbs[LDBL_MANT_BIT / GMP_LIMB_BITS] = (hi << (GMP_LIMB_BITS / 2)) | lo;
    }
#   else
    {
      mp_limb_t d;
      y *= (mp_limb_t) 1 << (LDBL_MANT_BIT % GMP_LIMB_BITS);
      d = (int) y;
      y -= d;
      if (!(y >= 0.0L && y < 1.0L))
        abort ();
      m.limbs[LDBL_MANT_BIT / GMP_LIMB_BITS] = d;
    }
#   endif
#  endif
  for (i = LDBL_MANT_BIT / GMP_LIMB_BITS; i > 0; )
    {
      mp_limb_t hi, lo;
      y *= (mp_limb_t) 1 << (GMP_LIMB_BITS / 2);
      hi = (int) y;
      y -= hi;
      if (!(y >= 0.0L && y < 1.0L))
        abort ();
      y *= (mp_limb_t) 1 << (GMP_LIMB_BITS / 2);
      lo = (int) y;
      y -= lo;
      if (!(y >= 0.0L && y < 1.0L))
        abort ();
      m.limbs[--i] = (hi << (GMP_LIMB_BITS / 2)) | lo;
    }
#  if 0  
  if (!(y == 0.0L))
    abort ();
#  endif
   
  while (m.nlimbs > 0 && m.limbs[m.nlimbs - 1] == 0)
    m.nlimbs--;
  *mp = m;
  *ep = exp - LDBL_MANT_BIT;
  return m.limbs;
}
