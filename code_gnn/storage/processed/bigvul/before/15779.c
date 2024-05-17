void ahci_uninit(AHCIState *s)
{
    memory_region_destroy(&s->mem);
    memory_region_destroy(&s->idp);
    g_free(s->dev);
}
