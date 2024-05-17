test_bson_append_minkey (void)
{
   bson_t *b;
   bson_t *b2;

   b = bson_new ();
   BSON_ASSERT (bson_append_minkey (b, "minkey", -1));
   b2 = get_bson ("test36.bson");
   BSON_ASSERT_BSON_EQUAL (b, b2);
   bson_destroy (b);
   bson_destroy (b2);
}