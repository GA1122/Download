ProcScreenSaverSuspend(ClientPtr client)
{
    ScreenSaverSuspensionPtr *prev, this;

    REQUEST(xScreenSaverSuspendReq);
    REQUEST_SIZE_MATCH(xScreenSaverSuspendReq);

     
    for (prev = &suspendingClients; (this = *prev); prev = &this->next)
        if (this->pClient == client)
            break;

    if (this) {
        if (stuff->suspend == TRUE)
            this->count++;
        else if (--this->count == 0)
            FreeResource(this->clientResource, RT_NONE);

        return Success;
    }

     
    if (stuff->suspend == FALSE)
        return Success;

     
    this = malloc(sizeof(ScreenSaverSuspensionRec));

    if (!this)
        return BadAlloc;

    this->next = NULL;
    this->pClient = client;
    this->count = 1;
    this->clientResource = FakeClientID(client->index);

    if (!AddResource(this->clientResource, SuspendType, (void *) this)) {
        free(this);
        return BadAlloc;
    }

    *prev = this;
    if (!screenSaverSuspended) {
        screenSaverSuspended = TRUE;
        FreeScreenSaverTimer();
    }

    return Success;
}
