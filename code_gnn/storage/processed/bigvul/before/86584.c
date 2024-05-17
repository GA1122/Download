crm_ipc_read(crm_ipc_t * client)
{
    struct crm_ipc_response_header *header = NULL;

    CRM_ASSERT(client != NULL);
    CRM_ASSERT(client->ipc != NULL);
    CRM_ASSERT(client->buffer != NULL);

    crm_ipc_init();

    client->buffer[0] = 0;
    client->msg_size = qb_ipcc_event_recv(client->ipc, client->buffer, client->buf_size - 1, 0);
    if (client->msg_size >= 0) {
        int rc = crm_ipc_decompress(client);

        if (rc != pcmk_ok) {
            return rc;
        }

        header = (struct crm_ipc_response_header *)(void*)client->buffer;
        if(header->version > PCMK_IPC_VERSION) {
            crm_err("Filtering incompatible v%d IPC message, we only support versions <= %d",
                    header->version, PCMK_IPC_VERSION);
            return -EBADMSG;
        }

        crm_trace("Received %s event %d, size=%u, rc=%d, text: %.100s",
                  client->name, header->qb.id, header->qb.size, client->msg_size,
                  client->buffer + hdr_offset);

    } else {
        crm_trace("No message from %s received: %s", client->name, pcmk_strerror(client->msg_size));
    }

    if (crm_ipc_connected(client) == FALSE || client->msg_size == -ENOTCONN) {
        crm_err("Connection to %s failed", client->name);
    }

    if (header) {
         
        return header->size_uncompressed;
    }
    return -ENOMSG;
}