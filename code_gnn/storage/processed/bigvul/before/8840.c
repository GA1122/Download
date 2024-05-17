static void vrend_destroy_query(struct vrend_query *query)
{
   vrend_resource_reference(&query->res, NULL);
   list_del(&query->waiting_queries);
   glDeleteQueries(1, &query->id);
   free(query);
}
