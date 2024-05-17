PanoramiXRenderCreateSolidFill (ClientPtr client)
{
    REQUEST(xRenderCreateSolidFillReq);
    PanoramiXRes    *newPict;
    int		    result = Success, j;

    REQUEST_AT_LEAST_SIZE(xRenderCreateSolidFillReq);

    if(!(newPict = (PanoramiXRes *) malloc(sizeof(PanoramiXRes))))
	return BadAlloc;

    newPict->type = XRT_PICTURE;
    newPict->info[0].id = stuff->pid;
    newPict->u.pict.root = FALSE;

    for(j = 1; j < PanoramiXNumScreens; j++)
	newPict->info[j].id = FakeClientID(client->index);
	
    FOR_NSCREENS_BACKWARD(j) {
	stuff->pid = newPict->info[j].id;
	result = (*PanoramiXSaveRenderVector[X_RenderCreateSolidFill]) (client);
	if(result != Success) break;
    }

    if (result == Success)
	AddResource(newPict->info[0].id, XRT_PICTURE, newPict);
    else
	free(newPict);

    return result;
}