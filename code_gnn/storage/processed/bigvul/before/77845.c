bson_iter_next (bson_iter_t *iter)  
{
   uint32_t bson_type;
   const char *key;
   bool unsupported;

   return _bson_iter_next_internal (iter, 0, &key, &bson_type, &unsupported);
}
