bson_iter_overwrite_bool (bson_iter_t *iter,  
                          bool value)         
{
   BSON_ASSERT (iter);

   if (ITER_TYPE (iter) == BSON_TYPE_BOOL) {
      memcpy ((void *) (iter->raw + iter->d1), &value, 1);
   }
}
