bson_iter_dbpointer (const bson_iter_t *iter,   
                     uint32_t *collection_len,  
                     const char **collection,   
                     const bson_oid_t **oid)    
{
   BSON_ASSERT (iter);

   if (collection) {
      *collection = NULL;
   }

   if (oid) {
      *oid = NULL;
   }

   if (ITER_TYPE (iter) == BSON_TYPE_DBPOINTER) {
      if (collection_len) {
         memcpy (
            collection_len, (iter->raw + iter->d1), sizeof (*collection_len));
         *collection_len = BSON_UINT32_FROM_LE (*collection_len);

         if ((*collection_len) > 0) {
            (*collection_len)--;
         }
      }

      if (collection) {
         *collection = (const char *) (iter->raw + iter->d2);
      }

      if (oid) {
         *oid = (const bson_oid_t *) (iter->raw + iter->d3);
      }
   }
}
