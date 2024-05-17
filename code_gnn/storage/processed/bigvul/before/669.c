sparse_scan_file_wholesparse (struct tar_sparse_file *file)
{
  struct tar_stat_info *st = file->stat_info;
  struct sp_array sp = {0, 0};

   
  if (ST_NBLOCKS (st->stat) == 0)
    {
      st->archive_file_size = 0;
      sp.offset = st->stat.st_size;
      sparse_add_map (st, &sp);
      return true;
    }

  return false;
}
