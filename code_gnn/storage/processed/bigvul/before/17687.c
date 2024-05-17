ProcXFixesChangeSaveSet(ClientPtr client)
{
    Bool toRoot, map;
    int result;
    WindowPtr pWin;

    REQUEST(xXFixesChangeSaveSetReq);

    REQUEST_SIZE_MATCH(xXFixesChangeSaveSetReq);
    result = dixLookupWindow(&pWin, stuff->window, client, DixManageAccess);
    if (result != Success)
        return result;
    if (client->clientAsMask == (CLIENT_BITS(pWin->drawable.id)))
        return BadMatch;
    if ((stuff->mode != SetModeInsert) && (stuff->mode != SetModeDelete)) {
        client->errorValue = stuff->mode;
        return BadValue;
    }
    if ((stuff->target != SaveSetNearest) && (stuff->target != SaveSetRoot)) {
        client->errorValue = stuff->target;
        return BadValue;
    }
    if ((stuff->map != SaveSetMap) && (stuff->map != SaveSetUnmap)) {
        client->errorValue = stuff->map;
        return BadValue;
    }
    toRoot = (stuff->target == SaveSetRoot);
    map = (stuff->map == SaveSetMap);
    return AlterSaveSetForClient(client, pWin, stuff->mode, toRoot, map);
}
