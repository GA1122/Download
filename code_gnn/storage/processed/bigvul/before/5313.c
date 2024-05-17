star_fixup_header (struct tar_sparse_file *file)
{
   
  off_t realsize = OFF_FROM_HEADER (current_header->star_in_header.realsize);
  file->stat_info->archive_file_size = file->stat_info->stat.st_size;
  file->stat_info->stat.st_size = max (0, realsize);
  return 0 <= realsize;
}
