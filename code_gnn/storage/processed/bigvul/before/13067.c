ec_pow2 (gcry_mpi_t w, const gcry_mpi_t b, mpi_ec_t ctx)
{
   
   
  ec_mulm (w, b, b, ctx);
}
