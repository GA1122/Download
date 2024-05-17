ProcRenderQueryPictFormats(ClientPtr client)
{
    RenderClientPtr pRenderClient = GetRenderClient(client);
    xRenderQueryPictFormatsReply *reply;
    xPictScreen *pictScreen;
    xPictDepth *pictDepth;
    xPictVisual *pictVisual;
    xPictFormInfo *pictForm;
    CARD32 *pictSubpixel;
    ScreenPtr pScreen;
    VisualPtr pVisual;
    DepthPtr pDepth;
    int v, d;
    PictureScreenPtr ps;
    PictFormatPtr pFormat;
    int nformat;
    int ndepth;
    int nvisual;
    int rlength;
    int s;
    int numScreens;
    int numSubpixel;

 

    REQUEST_SIZE_MATCH(xRenderQueryPictFormatsReq);

#ifdef PANORAMIX
    if (noPanoramiXExtension)
        numScreens = screenInfo.numScreens;
    else
        numScreens = ((xConnSetup *) ConnectionInfo)->numRoots;
#else
    numScreens = screenInfo.numScreens;
#endif
    ndepth = nformat = nvisual = 0;
    for (s = 0; s < numScreens; s++) {
        pScreen = screenInfo.screens[s];
        for (d = 0; d < pScreen->numDepths; d++) {
            pDepth = pScreen->allowedDepths + d;
            ++ndepth;

            for (v = 0; v < pDepth->numVids; v++) {
                pVisual = findVisual(pScreen, pDepth->vids[v]);
                if (pVisual &&
                    PictureMatchVisual(pScreen, pDepth->depth, pVisual))
                    ++nvisual;
            }
        }
        ps = GetPictureScreenIfSet(pScreen);
        if (ps)
            nformat += ps->nformats;
    }
    if (pRenderClient->major_version == 0 && pRenderClient->minor_version < 6)
        numSubpixel = 0;
    else
        numSubpixel = numScreens;

    rlength = (sizeof(xRenderQueryPictFormatsReply) +
               nformat * sizeof(xPictFormInfo) +
               numScreens * sizeof(xPictScreen) +
               ndepth * sizeof(xPictDepth) +
               nvisual * sizeof(xPictVisual) + numSubpixel * sizeof(CARD32));
    reply = (xRenderQueryPictFormatsReply *) calloc(1, rlength);
    if (!reply)
        return BadAlloc;
    reply->type = X_Reply;
    reply->sequenceNumber = client->sequence;
    reply->length = bytes_to_int32(rlength - sizeof(xGenericReply));
    reply->numFormats = nformat;
    reply->numScreens = numScreens;
    reply->numDepths = ndepth;
    reply->numVisuals = nvisual;
    reply->numSubpixel = numSubpixel;

    pictForm = (xPictFormInfo *) (reply + 1);

    for (s = 0; s < numScreens; s++) {
        pScreen = screenInfo.screens[s];
        ps = GetPictureScreenIfSet(pScreen);
        if (ps) {
            for (nformat = 0, pFormat = ps->formats;
                 nformat < ps->nformats; nformat++, pFormat++) {
                pictForm->id = pFormat->id;
                pictForm->type = pFormat->type;
                pictForm->depth = pFormat->depth;
                pictForm->direct.red = pFormat->direct.red;
                pictForm->direct.redMask = pFormat->direct.redMask;
                pictForm->direct.green = pFormat->direct.green;
                pictForm->direct.greenMask = pFormat->direct.greenMask;
                pictForm->direct.blue = pFormat->direct.blue;
                pictForm->direct.blueMask = pFormat->direct.blueMask;
                pictForm->direct.alpha = pFormat->direct.alpha;
                pictForm->direct.alphaMask = pFormat->direct.alphaMask;
                if (pFormat->type == PictTypeIndexed &&
                    pFormat->index.pColormap)
                    pictForm->colormap = pFormat->index.pColormap->mid;
                else
                    pictForm->colormap = None;
                if (client->swapped) {
                    swapl(&pictForm->id);
                    swaps(&pictForm->direct.red);
                    swaps(&pictForm->direct.redMask);
                    swaps(&pictForm->direct.green);
                    swaps(&pictForm->direct.greenMask);
                    swaps(&pictForm->direct.blue);
                    swaps(&pictForm->direct.blueMask);
                    swaps(&pictForm->direct.alpha);
                    swaps(&pictForm->direct.alphaMask);
                    swapl(&pictForm->colormap);
                }
                pictForm++;
            }
        }
    }

    pictScreen = (xPictScreen *) pictForm;
    for (s = 0; s < numScreens; s++) {
        pScreen = screenInfo.screens[s];
        pictDepth = (xPictDepth *) (pictScreen + 1);
        ndepth = 0;
        for (d = 0; d < pScreen->numDepths; d++) {
            pictVisual = (xPictVisual *) (pictDepth + 1);
            pDepth = pScreen->allowedDepths + d;

            nvisual = 0;
            for (v = 0; v < pDepth->numVids; v++) {
                pVisual = findVisual(pScreen, pDepth->vids[v]);
                if (pVisual && (pFormat = PictureMatchVisual(pScreen,
                                                             pDepth->depth,
                                                             pVisual))) {
                    pictVisual->visual = pVisual->vid;
                    pictVisual->format = pFormat->id;
                    if (client->swapped) {
                        swapl(&pictVisual->visual);
                        swapl(&pictVisual->format);
                    }
                    pictVisual++;
                    nvisual++;
                }
            }
            pictDepth->depth = pDepth->depth;
            pictDepth->nPictVisuals = nvisual;
            if (client->swapped) {
                swaps(&pictDepth->nPictVisuals);
            }
            ndepth++;
            pictDepth = (xPictDepth *) pictVisual;
        }
        pictScreen->nDepth = ndepth;
        ps = GetPictureScreenIfSet(pScreen);
        if (ps)
            pictScreen->fallback = ps->fallback->id;
        else
            pictScreen->fallback = 0;
        if (client->swapped) {
            swapl(&pictScreen->nDepth);
            swapl(&pictScreen->fallback);
        }
        pictScreen = (xPictScreen *) pictDepth;
    }
    pictSubpixel = (CARD32 *) pictScreen;

    for (s = 0; s < numSubpixel; s++) {
        pScreen = screenInfo.screens[s];
        ps = GetPictureScreenIfSet(pScreen);
        if (ps)
            *pictSubpixel = ps->subpixel;
        else
            *pictSubpixel = SubPixelUnknown;
        if (client->swapped) {
            swapl(pictSubpixel);
        }
        ++pictSubpixel;
    }

    if (client->swapped) {
        swaps(&reply->sequenceNumber);
        swapl(&reply->length);
        swapl(&reply->numFormats);
        swapl(&reply->numScreens);
        swapl(&reply->numDepths);
        swapl(&reply->numVisuals);
        swapl(&reply->numSubpixel);
    }
    WriteToClient(client, rlength, reply);
    free(reply);
    return Success;
}
