ConstructClientIds(ClientPtr client,
                   int numSpecs, xXResClientIdSpec* specs,
                   ConstructClientIdCtx *ctx)
{
    int specIdx;

    for (specIdx = 0; specIdx < numSpecs; ++specIdx) {
        if (specs[specIdx].client == 0) {
            int c;
            for (c = 0; c < currentMaxClients; ++c) {
                if (clients[c]) {
                    if (!ConstructClientIdValue(client, clients[c],
                                                specs[specIdx].mask, ctx)) {
                        return BadAlloc;
                    }
                }
            }
        } else {
            int clientID = CLIENT_ID(specs[specIdx].client);

            if ((clientID < currentMaxClients) && clients[clientID]) {
                if (!ConstructClientIdValue(client, clients[clientID],
                                            specs[specIdx].mask, ctx)) {
                    return BadAlloc;
                }
            }
        }
    }

     
    return Success;
}
