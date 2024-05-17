_gcry_mpi_point_set (mpi_point_t point,
                     gcry_mpi_t x, gcry_mpi_t y, gcry_mpi_t z)
{
  if (!point)
    point = mpi_point_new (0);

  if (x)
    mpi_set (point->x, x);
  else
    mpi_clear (point->x);
  if (y)
    mpi_set (point->y, y);
  else
    mpi_clear (point->y);
  if (z)
    mpi_set (point->z, z);
  else
    mpi_clear (point->z);

  return point;
}