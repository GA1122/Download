chunk_init(struct chunk * const chunk, struct file * const file)
  
{
   assert(file->chunk == NULL);

   CLEAR(*chunk);

   chunk->file = file;
   chunk->global = file->global;

   chunk->chunk_data_pos = file->data_pos;
   chunk->chunk_length = file->length;
   chunk->chunk_type = file->type;

  
   chunk->uncompressed_digits = 0;
   chunk->compressed_digits = 0;

   file->chunk = chunk;
}
