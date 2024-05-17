bson_iter_init_find_w_len (bson_iter_t *iter,   
                           const bson_t *bson,  
                           const char *key,     
                           int keylen)          
{
   BSON_ASSERT (iter);
   BSON_ASSERT (bson);
   BSON_ASSERT (key);

   return bson_iter_init (iter, bson) &&
          bson_iter_find_w_len (iter, key, keylen);
}
