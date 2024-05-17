static struct packet *packet_alloc(const uint8_t *data, uint32_t len)
{
 struct packet *p = osi_calloc(sizeof(*p));
 uint8_t *buf = osi_malloc(len);

 if (p && buf) {

        p->data = buf;
        p->len = len;
        memcpy(p->data, data, len);
 return p;

 } else if (p)
       osi_free(p);
 else if (buf)
       osi_free(buf);

 return NULL;
}
