static int scoop_post_load(void *opaque, int version_id)
{
    ScoopInfo *s = (ScoopInfo *) opaque;
    int i;
    uint32_t level;

    level = s->gpio_level & s->gpio_dir;

    for (i = 0; i < 16; i++) {
        qemu_set_irq(s->handler[i], (level >> i) & 1);
    }

    s->prev_level = level;

    return 0;
}
