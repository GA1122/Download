mainloop_gio_refcount(mainloop_io_t *client) 
{
     
    if(client && client->channel) {
        int *ref = (void*)client->channel;
        return *ref;
    }
    return 0;
}
