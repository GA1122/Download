bson_iter_init_find (bson_iter_t *iter,   
                     const bson_t *bson,  
                     const char *key)     
{
   BSON_ASSERT (iter);
   BSON_ASSERT (bson);
   BSON_ASSERT (key);

   return bson_iter_init (iter, bson) && bson_iter_find (iter, key);
}
