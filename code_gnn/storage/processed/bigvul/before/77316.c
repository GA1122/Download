ofproto_get_n_visible_tables(const struct ofproto *ofproto)
{
    uint8_t n = ofproto->n_tables;

     
    while(n && (ofproto->tables[n - 1].flags & OFTABLE_HIDDEN)) {
        n--;
    }

    return n;
}
