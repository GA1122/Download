_gcry_mpi_ec_get_reset (mpi_ec_t ec)
{
  ec->t.valid.a_is_pminus3 = 0;
  ec->t.valid.two_inv_p = 0;
}
