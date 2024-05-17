setpos(struct chunk *chunk)
  
{
   chunk->file->read_count = 8;
   file_setpos(chunk->file, &chunk->chunk_data_pos);
}
