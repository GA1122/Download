gen_x931_parm_xp (unsigned int nbits)
{
  gcry_mpi_t xp;

  xp = mpi_snew (nbits);
  _gcry_mpi_randomize (xp, nbits, GCRY_VERY_STRONG_RANDOM);

   
  mpi_set_highbit (xp, nbits-1);
  mpi_set_bit (xp, nbits-2);
  gcry_assert ( mpi_get_nbits (xp) == nbits );

  return xp;
}
