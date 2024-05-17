ec_get_two_inv_p (mpi_ec_t ec)
{
  if (!ec->t.valid.two_inv_p)
    {
      ec->t.valid.two_inv_p = 1;
      if (!ec->t.two_inv_p)
        ec->t.two_inv_p = mpi_alloc (0);
      ec_invm (ec->t.two_inv_p, mpi_const (MPI_C_TWO), ec);
    }
  return ec->t.two_inv_p;
 }
