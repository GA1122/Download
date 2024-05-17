gio_poll_dispatch_add(enum qb_loop_priority p, int32_t fd, int32_t evts,
                  void *data, qb_ipcs_dispatch_fn_t fn)
{
    struct gio_to_qb_poll *adaptor;
    GIOChannel *channel;
    int32_t res = 0;

    res = qb_array_index(gio_map, fd, (void**)&adaptor);
    if (res < 0) {
        crm_err("Array lookup failed for fd=%d: %d", fd, res);
        return res;
    }

    crm_trace("Adding fd=%d to mainloop as adapater %p", fd, adaptor);
    if (adaptor->is_used) {
        crm_err("Adapter for descriptor %d is still in-use", fd);
        return -EEXIST;
    }

     
    channel = g_io_channel_unix_new(fd);
    if (!channel) {
        crm_err("No memory left to add fd=%d", fd);
        return -ENOMEM;
    }

     
    evts |= (G_IO_HUP|G_IO_NVAL|G_IO_ERR);

    adaptor->channel = channel;
    adaptor->fn = fn;
    adaptor->events = evts;
    adaptor->data = data;
    adaptor->p = p;
    adaptor->is_used = QB_TRUE;
    adaptor->source = g_io_add_watch_full(channel, G_PRIORITY_DEFAULT, evts, gio_read_socket, adaptor, gio_poll_destroy);

     
    g_io_channel_unref(adaptor->channel);    

    crm_trace("Added to mainloop with gsource id=%d, ref=%d", adaptor->source, gio_adapter_refcount(adaptor));
    if(adaptor->source > 0) {
        return 0;
    }
    
    return -EINVAL;
}