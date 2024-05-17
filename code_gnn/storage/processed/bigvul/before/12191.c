void qemu_spice_destroy_update(SimpleSpiceDisplay *sdpy, SimpleSpiceUpdate *update)
{
    g_free(update->bitmap);
    g_free(update);
}
