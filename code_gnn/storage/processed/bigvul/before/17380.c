ProcXineramaQueryScreens(ClientPtr client)
{
     
    CARD32 number = (noPanoramiXExtension) ? 0 : PanoramiXNumScreens;
    xXineramaQueryScreensReply rep = {
        .type = X_Reply,
        .sequenceNumber = client->sequence,
        .length = bytes_to_int32(number * sz_XineramaScreenInfo),
        .number = number
    };

    REQUEST_SIZE_MATCH(xXineramaQueryScreensReq);

    if (client->swapped) {
        swaps(&rep.sequenceNumber);
        swapl(&rep.length);
        swapl(&rep.number);
    }
    WriteToClient(client, sizeof(xXineramaQueryScreensReply), &rep);

    if (!noPanoramiXExtension) {
        xXineramaScreenInfo scratch;
        int i;

        FOR_NSCREENS(i) {
            scratch.x_org = screenInfo.screens[i]->x;
            scratch.y_org = screenInfo.screens[i]->y;
            scratch.width = screenInfo.screens[i]->width;
            scratch.height = screenInfo.screens[i]->height;

            if (client->swapped) {
                swaps(&scratch.x_org);
                swaps(&scratch.y_org);
                swaps(&scratch.width);
                swaps(&scratch.height);
            }
            WriteToClient(client, sz_XineramaScreenInfo, &scratch);
        }
    }

    return Success;
}
