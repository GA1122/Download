ProcChangeCloseDownMode(ClientPtr client)
{
    int rc;

    REQUEST(xSetCloseDownModeReq);
    REQUEST_SIZE_MATCH(xSetCloseDownModeReq);

    rc = XaceHook(XACE_CLIENT_ACCESS, client, client, DixManageAccess);
    if (rc != Success)
        return rc;

    if ((stuff->mode == AllTemporary) ||
        (stuff->mode == RetainPermanent) || (stuff->mode == RetainTemporary)) {
        client->closeDownMode = stuff->mode;
        return Success;
    }
    else {
        client->errorValue = stuff->mode;
        return BadValue;
    }
}
