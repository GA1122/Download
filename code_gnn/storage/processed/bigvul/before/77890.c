test_bson_append_overflow (void)
{
   const char *key = "a";
   uint32_t len;
   bson_t b;

   len = BSON_MAX_SIZE;
   len -= 4;  
   len -= 1;  
   len -= 1;  
   len -= 1;  

   bson_init (&b);
   BSON_ASSERT (!bson_append_bool (&b, key, len, true));
   bson_destroy (&b);
}
