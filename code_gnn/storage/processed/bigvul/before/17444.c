ProcXResQueryClients(ClientPtr client)
{
     
    xXResQueryClientsReply rep;
    int *current_clients;
    int i, num_clients;

    REQUEST_SIZE_MATCH(xXResQueryClientsReq);

    current_clients = xallocarray(currentMaxClients, sizeof(int));

    num_clients = 0;
    for (i = 0; i < currentMaxClients; i++) {
        if (clients[i]) {
            current_clients[num_clients] = i;
            num_clients++;
        }
    }

    rep = (xXResQueryClientsReply) {
        .type = X_Reply,
        .sequenceNumber = client->sequence,
        .length = bytes_to_int32(num_clients * sz_xXResClient),
        .num_clients = num_clients
    };
    if (client->swapped) {
        swaps(&rep.sequenceNumber);
        swapl(&rep.length);
        swapl(&rep.num_clients);
    }
    WriteToClient(client, sizeof(xXResQueryClientsReply), &rep);

    if (num_clients) {
        xXResClient scratch;

        for (i = 0; i < num_clients; i++) {
            scratch.resource_base = clients[current_clients[i]]->clientAsMask;
            scratch.resource_mask = RESOURCE_ID_MASK;

            if (client->swapped) {
                swapl(&scratch.resource_base);
                swapl(&scratch.resource_mask);
            }
            WriteToClient(client, sz_xXResClient, &scratch);
        }
    }

    free(current_clients);

    return Success;
}
