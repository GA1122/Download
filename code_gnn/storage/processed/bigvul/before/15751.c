 static int hpet_post_load(void *opaque, int version_id)
 {
     HPETState *s = opaque;
        s->flags |= 1 << HPET_MSI_SUPPORT;
    }
