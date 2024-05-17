bson_iter_code (const bson_iter_t *iter,  
                uint32_t *length)         
{
   BSON_ASSERT (iter);

   if (ITER_TYPE (iter) == BSON_TYPE_CODE) {
      if (length) {
         *length = bson_iter_utf8_len_unsafe (iter);
      }

      return (const char *) (iter->raw + iter->d2);
   }

   if (length) {
      *length = 0;
   }

   return NULL;
}
