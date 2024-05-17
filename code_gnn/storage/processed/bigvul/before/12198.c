 static uint32_t vbe_ioport_read_index(void *opaque, uint32_t addr)
 {
     VGACommonState *s = opaque;
              val &= s->vbe_bank_mask;
            }
