gio_poll_destroy(gpointer data) 
{
     
    struct gio_to_qb_poll *adaptor = (struct gio_to_qb_poll *)data;

    crm_trace("Destroying adaptor %p channel %p (ref=%d)", adaptor, adaptor->channel, gio_adapter_refcount(adaptor));
    adaptor->is_used = QB_FALSE;
    adaptor->channel = NULL;
    adaptor->source = 0;
}
