CloseDownClient(ClientPtr client)
{
    Bool really_close_down = client->clientGone ||
        client->closeDownMode == DestroyAll;

    if (!client->clientGone) {
         
        if (grabState != GrabNone && grabClient == client) {
            UngrabServer(client);
        }
        BITCLEAR(grabWaiters, client->index);
        DeleteClientFromAnySelections(client);
        ReleaseActiveGrabs(client);
        DeleteClientFontStuff(client);
        if (!really_close_down) {
             
            FreeClientNeverRetainResources(client);
            client->clientState = ClientStateRetained;
            if (ClientStateCallback) {
                NewClientInfoRec clientinfo;

                clientinfo.client = client;
                clientinfo.prefix = (xConnSetupPrefix *) NULL;
                clientinfo.setup = (xConnSetup *) NULL;
                CallCallbacks((&ClientStateCallback), (void *) &clientinfo);
            }
        }
        client->clientGone = TRUE;       
        if (ClientIsAsleep(client))
            ClientSignal(client);
        ProcessWorkQueueZombies();
        CloseDownConnection(client);
        output_pending_clear(client);
        mark_client_not_ready(client);

         
        if (client->clientState != ClientStateInitial) {
            --nClients;
        }
    }

    if (really_close_down) {
        if (client->clientState == ClientStateRunning && nClients == 0)
            dispatchException |= dispatchExceptionAtReset;

        client->clientState = ClientStateGone;
        if (ClientStateCallback) {
            NewClientInfoRec clientinfo;

            clientinfo.client = client;
            clientinfo.prefix = (xConnSetupPrefix *) NULL;
            clientinfo.setup = (xConnSetup *) NULL;
            CallCallbacks((&ClientStateCallback), (void *) &clientinfo);
        }
        TouchListenerGone(client->clientAsMask);
        FreeClientResources(client);
         
        ReleaseClientIds(client);
#ifdef XSERVER_DTRACE
        XSERVER_CLIENT_DISCONNECT(client->index);
#endif
        if (client->index < nextFreeClientID)
            nextFreeClientID = client->index;
        clients[client->index] = NullClient;
        SmartLastClient = NullClient;
        dixFreeObjectWithPrivates(client, PRIVATE_CLIENT);

        while (!clients[currentMaxClients - 1])
            currentMaxClients--;
    }
}
