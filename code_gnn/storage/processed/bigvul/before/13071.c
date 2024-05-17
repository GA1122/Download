point_resize (mpi_point_t p, mpi_ec_t ctx)
{
   
  size_t nlimbs = 2*ctx->p->nlimbs+1;

  mpi_resize (p->x, nlimbs);
  if (ctx->model != MPI_EC_MONTGOMERY)
    mpi_resize (p->y, nlimbs);
  mpi_resize (p->z, nlimbs);
}
