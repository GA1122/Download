static void rng_backend_finalize(Object *obj)
{
    RngBackend *s = RNG_BACKEND(obj);

    rng_backend_free_requests(s);
}
