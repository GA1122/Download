mainloop_gio_destroy(gpointer c)
{
    mainloop_io_t *client = c;

     
    crm_trace("Destroying client %s[%p] %d", client->name, c, mainloop_gio_refcount(client));

    if(client->ipc) {
        crm_ipc_close(client->ipc);
    }

    if(client->destroy_fn) {
        client->destroy_fn(client->userdata);
    }
    
    if(client->ipc) {
        crm_ipc_destroy(client->ipc);
    }

    crm_trace("Destroyed client %s[%p] %d", client->name, c, mainloop_gio_refcount(client));
    free(client->name);

    memset(client, 0, sizeof(mainloop_io_t));  
    free(client);
}
