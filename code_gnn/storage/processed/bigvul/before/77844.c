bson_iter_key_len (const bson_iter_t *iter)
{
    
   BSON_ASSERT (iter->d1 > iter->key);
   return iter->d1 - iter->key - 1;
}