ProcKillClient(ClientPtr client)
{
    REQUEST(xResourceReq);
    ClientPtr killclient;
    int rc;

    REQUEST_SIZE_MATCH(xResourceReq);
    if (stuff->id == AllTemporary) {
        CloseDownRetainedResources();
        return Success;
    }

    rc = dixLookupClient(&killclient, stuff->id, client, DixDestroyAccess);
    if (rc == Success) {
        CloseDownClient(killclient);
        if (client == killclient) {
             
            isItTimeToYield = TRUE;
        }
        return Success;
    }
    else
        return rc;
}
