pax_dump_header_1 (struct tar_sparse_file *file)
{
  off_t block_ordinal = current_block_ordinal ();
  union block *blk;
  char *p, *q;
  size_t i;
  char nbuf[UINTMAX_STRSIZE_BOUND];
  off_t size = 0;
  struct sp_array *map = file->stat_info->sparse_map;
  char *save_file_name = file->stat_info->file_name;

#define COPY_STRING(b,dst,src) do                \
 {                                               \
   char *endp = b->buffer + BLOCKSIZE;           \
   char const *srcp = src;                       \
   while (*srcp)                                 \
     {                                           \
       if (dst == endp)                          \
	 {                                       \
	   set_next_block_after (b);             \
	   b = find_next_block ();               \
           dst = b->buffer;                      \
	   endp = b->buffer + BLOCKSIZE;         \
	 }                                       \
       *dst++ = *srcp++;                         \
     }                                           \
   } while (0)

   
  p = umaxtostr (file->stat_info->sparse_map_avail, nbuf);
  size += strlen (p) + 1;
  for (i = 0; i < file->stat_info->sparse_map_avail; i++)
    {
      p = umaxtostr (map[i].offset, nbuf);
      size += strlen (p) + 1;
      p = umaxtostr (map[i].numbytes, nbuf);
      size += strlen (p) + 1;
    }
  size = (size + BLOCKSIZE - 1) / BLOCKSIZE;
  file->stat_info->archive_file_size += size * BLOCKSIZE;
  file->dumped_size += size * BLOCKSIZE;

   
  xheader_store ("GNU.sparse.major", file->stat_info, NULL);
  xheader_store ("GNU.sparse.minor", file->stat_info, NULL);
  xheader_store ("GNU.sparse.name", file->stat_info, NULL);
  xheader_store ("GNU.sparse.realsize", file->stat_info, NULL);

  file->stat_info->file_name =
    xheader_format_name (file->stat_info, "%d/GNUSparseFile.%p/%f", 0);
   
  if (strlen (file->stat_info->file_name) > NAME_FIELD_SIZE)
    file->stat_info->file_name[NAME_FIELD_SIZE] = 0;

  blk = pax_start_header (file->stat_info);
  finish_header (file->stat_info, blk, block_ordinal);
  free (file->stat_info->file_name);
  file->stat_info->file_name = save_file_name;

  blk = find_next_block ();
  q = blk->buffer;
  p = umaxtostr (file->stat_info->sparse_map_avail, nbuf);
  COPY_STRING (blk, q, p);
  COPY_STRING (blk, q, "\n");
  for (i = 0; i < file->stat_info->sparse_map_avail; i++)
    {
      p = umaxtostr (map[i].offset, nbuf);
      COPY_STRING (blk, q, p);
      COPY_STRING (blk, q, "\n");
      p = umaxtostr (map[i].numbytes, nbuf);
      COPY_STRING (blk, q, p);
      COPY_STRING (blk, q, "\n");
    }
  memset (q, 0, BLOCKSIZE - (q - blk->buffer));
  set_next_block_after (blk);
  return true;
}
