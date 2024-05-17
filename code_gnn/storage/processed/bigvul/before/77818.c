bson_iter_array (const bson_iter_t *iter,  
                 uint32_t *array_len,      
                 const uint8_t **array)    
{
   BSON_ASSERT (iter);
   BSON_ASSERT (array_len);
   BSON_ASSERT (array);

   *array = NULL;
   *array_len = 0;

   if (ITER_TYPE (iter) == BSON_TYPE_ARRAY) {
      memcpy (array_len, (iter->raw + iter->d1), sizeof (*array_len));
      *array_len = BSON_UINT32_FROM_LE (*array_len);
      *array = (iter->raw + iter->d1);
   }
}
