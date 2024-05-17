static void scoop_gpio_set(void *opaque, int line, int level)
{
    ScoopInfo *s = (ScoopInfo *) opaque;

    if (level)
        s->gpio_level |= (1 << line);
    else
        s->gpio_level &= ~(1 << line);
}
