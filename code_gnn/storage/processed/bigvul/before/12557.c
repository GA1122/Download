static void rng_backend_free_request(RngRequest *req)
{
    g_free(req->data);
    g_free(req);
}
