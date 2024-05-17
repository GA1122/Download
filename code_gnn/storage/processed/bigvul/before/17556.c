PanoramiXRenderCreateRadialGradient(ClientPtr client)
{
    REQUEST(xRenderCreateRadialGradientReq);
    PanoramiXRes *newPict;
    int result = Success, j;

    REQUEST_AT_LEAST_SIZE(xRenderCreateRadialGradientReq);

    if (!(newPict = (PanoramiXRes *) malloc(sizeof(PanoramiXRes))))
        return BadAlloc;

    newPict->type = XRT_PICTURE;
    panoramix_setup_ids(newPict, client, stuff->pid);
    newPict->u.pict.root = FALSE;

    FOR_NSCREENS_BACKWARD(j) {
        stuff->pid = newPict->info[j].id;
        result =
            (*PanoramiXSaveRenderVector[X_RenderCreateRadialGradient]) (client);
        if (result != Success)
            break;
    }

    if (result == Success)
        AddResource(newPict->info[0].id, XRT_PICTURE, newPict);
    else
        free(newPict);

    return result;
}