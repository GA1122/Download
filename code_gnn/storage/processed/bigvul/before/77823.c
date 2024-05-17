bson_iter_bool (const bson_iter_t *iter)  
{
   BSON_ASSERT (iter);

   if (ITER_TYPE (iter) == BSON_TYPE_BOOL) {
      return bson_iter_bool_unsafe (iter);
   }

   return false;
}
