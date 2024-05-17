ProcChangeAccessControl(ClientPtr client)
{
    REQUEST(xSetAccessControlReq);

    REQUEST_SIZE_MATCH(xSetAccessControlReq);
    if ((stuff->mode != EnableAccess) && (stuff->mode != DisableAccess)) {
        client->errorValue = stuff->mode;
        return BadValue;
    }
    return ChangeAccessControl(client, stuff->mode == EnableAccess);
}
