bson_iter_timeval (const bson_iter_t *iter,  
                   struct timeval *tv)       
{
   BSON_ASSERT (iter);

   if (ITER_TYPE (iter) == BSON_TYPE_DATE_TIME) {
      bson_iter_timeval_unsafe (iter, tv);
      return;
   }

   memset (tv, 0, sizeof *tv);
}