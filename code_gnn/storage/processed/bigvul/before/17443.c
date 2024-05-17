ProcXResQueryClientResources(ClientPtr client)
{
    REQUEST(xXResQueryClientResourcesReq);
    xXResQueryClientResourcesReply rep;
    int i, clientID, num_types;
    int *counts;

    REQUEST_SIZE_MATCH(xXResQueryClientResourcesReq);

    clientID = CLIENT_ID(stuff->xid);

    if ((clientID >= currentMaxClients) || !clients[clientID]) {
        client->errorValue = stuff->xid;
        return BadValue;
    }

    counts = calloc(lastResourceType + 1, sizeof(int));

    FindAllClientResources(clients[clientID], ResFindAllRes, counts);

    num_types = 0;

    for (i = 0; i <= lastResourceType; i++) {
        if (counts[i])
            num_types++;
    }

    rep = (xXResQueryClientResourcesReply) {
        .type = X_Reply,
        .sequenceNumber = client->sequence,
        .length = bytes_to_int32(num_types * sz_xXResType),
        .num_types = num_types
    };
    if (client->swapped) {
        swaps(&rep.sequenceNumber);
        swapl(&rep.length);
        swapl(&rep.num_types);
    }

    WriteToClient(client, sizeof(xXResQueryClientResourcesReply), &rep);

    if (num_types) {
        xXResType scratch;
        const char *name;

        for (i = 0; i < lastResourceType; i++) {
            if (!counts[i])
                continue;

            name = LookupResourceName(i + 1);
            if (strcmp(name, XREGISTRY_UNKNOWN))
                scratch.resource_type = MakeAtom(name, strlen(name), TRUE);
            else {
                char buf[40];

                snprintf(buf, sizeof(buf), "Unregistered resource %i", i + 1);
                scratch.resource_type = MakeAtom(buf, strlen(buf), TRUE);
            }

            scratch.count = counts[i];

            if (client->swapped) {
                swapl(&scratch.resource_type);
                swapl(&scratch.count);
            }
            WriteToClient(client, sz_xXResType, &scratch);
        }
    }

    free(counts);

    return Success;
}
