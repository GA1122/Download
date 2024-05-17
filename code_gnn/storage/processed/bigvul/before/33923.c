mainloop_gio_callback(GIOChannel *gio, GIOCondition condition, gpointer data)
{
    gboolean keep = TRUE;
    mainloop_io_t *client = data;

    if(condition & G_IO_IN) {
        if(client->ipc) {
            long rc = 0;
            int max = 10;
            do {
                rc = crm_ipc_read(client->ipc);
                if(rc <= 0) {
                    crm_trace("Message acquisition from %s[%p] failed: %s (%ld)",
                              client->name, client, pcmk_strerror(rc), rc);

                } else if(client->dispatch_fn_ipc) {
                    const char *buffer = crm_ipc_buffer(client->ipc);
                    crm_trace("New message from %s[%p] = %d", client->name, client, rc, condition);
                    if(client->dispatch_fn_ipc(buffer, rc, client->userdata) < 0) {
                        crm_trace("Connection to %s no longer required", client->name);
                        keep = FALSE;
                    }
                }

            } while(keep && rc > 0 && --max > 0);

        } else {
            crm_trace("New message from %s[%p]", client->name, client);
            if(client->dispatch_fn_io) {
                if(client->dispatch_fn_io(client->userdata) < 0) {
                    crm_trace("Connection to %s no longer required", client->name);
                    keep = FALSE;
                }
            }
        }
    }

    if(client->ipc && crm_ipc_connected(client->ipc) == FALSE) {
        crm_err("Connection to %s[%p] closed (I/O condition=%d)", client->name, client, condition);
        keep = FALSE;

    } else if(condition & (G_IO_HUP|G_IO_NVAL|G_IO_ERR)) {
        crm_trace("The connection %s[%p] has been closed (I/O condition=%d, refcount=%d)",
                  client->name, client, condition, mainloop_gio_refcount(client));
        keep = FALSE;

    } else if((condition & G_IO_IN) == 0) {
         
        crm_err("Strange condition: %d", condition);
    }

     
    return keep;
}
