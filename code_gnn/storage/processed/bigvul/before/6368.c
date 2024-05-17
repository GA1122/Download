e1000_receive(NetClientState *nc, const uint8_t *buf, size_t size)
{
    E1000State *s = DO_UPCAST(NICState, nc, nc)->opaque;
    struct e1000_rx_desc desc;
    dma_addr_t base;
    unsigned int n, rdt;
    uint32_t rdh_start;
    uint16_t vlan_special = 0;
    uint8_t vlan_status = 0, vlan_offset = 0;
    uint8_t min_buf[MIN_BUF_SIZE];
    size_t desc_offset;
    size_t desc_size;
    size_t total_size;

    if (!(s->mac_reg[RCTL] & E1000_RCTL_EN))
        return -1;

     
    if (size < sizeof(min_buf)) {
        memcpy(min_buf, buf, size);
        memset(&min_buf[size], 0, sizeof(min_buf) - size);
        buf = min_buf;
        size = sizeof(min_buf);
    }

         size = sizeof(min_buf);
     }
