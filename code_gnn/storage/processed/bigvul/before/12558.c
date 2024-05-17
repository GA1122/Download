static void rng_backend_free_requests(RngBackend *s)
{
    GSList *i;

    for (i = s->requests; i; i = i->next) {
        rng_backend_free_request(i->data);
    }

    g_slist_free(s->requests);
    s->requests = NULL;
}
