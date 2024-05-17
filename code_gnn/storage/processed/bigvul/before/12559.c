static void rng_backend_init(Object *obj)
{
    object_property_add_bool(obj, "opened",
                             rng_backend_prop_get_opened,
                             rng_backend_prop_set_opened,
                             NULL);
}
