free_cell_pool(void)
{
   
  if (cell_pool) {
    mp_pool_destroy(cell_pool);
    cell_pool = NULL;
  }
}
