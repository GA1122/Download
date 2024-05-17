test_bson_visit_invalid_field (void)
{
    
   const char data[] = "\x0c\x00\x00\x00\x10\x80\x00\x01\x00\x00\x00\x00";
   bson_t b;
   bson_iter_t iter;
   bson_visitor_t visitor = {0};
   bool visited = false;

   visitor.visit_corrupt = visit_corrupt;
   BSON_ASSERT (bson_init_static (&b, (const uint8_t *) data, sizeof data - 1));
   BSON_ASSERT (bson_iter_init (&iter, &b));
   BSON_ASSERT (!bson_iter_visit_all (&iter, &visitor, (void *) &visited));
   BSON_ASSERT (visited);
}
