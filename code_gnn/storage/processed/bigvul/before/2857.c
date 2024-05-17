ProcPanoramiXShmCreatePixmap(ClientPtr client)
{
    ScreenPtr pScreen = NULL;
    PixmapPtr pMap = NULL;
    DrawablePtr pDraw;
    DepthPtr pDepth;
    int i, j, result, rc;
    ShmDescPtr shmdesc;

    REQUEST(xShmCreatePixmapReq);
    unsigned int width, height, depth;
    unsigned long size;
    PanoramiXRes *newPix;

    REQUEST_SIZE_MATCH(xShmCreatePixmapReq);
    client->errorValue = stuff->pid;
    if (!sharedPixmaps)
        return BadImplementation;
    LEGAL_NEW_RESOURCE(stuff->pid, client);
    rc = dixLookupDrawable(&pDraw, stuff->drawable, client, M_ANY,
                           DixGetAttrAccess);
    if (rc != Success)
        return rc;

    VERIFY_SHMPTR(stuff->shmseg, stuff->offset, TRUE, shmdesc, client);

    width = stuff->width;
    height = stuff->height;
    depth = stuff->depth;
    if (!width || !height || !depth) {
        client->errorValue = 0;
        return BadValue;
    }
    if (width > 32767 || height > 32767)
        return BadAlloc;

    if (stuff->depth != 1) {
        pDepth = pDraw->pScreen->allowedDepths;
        for (i = 0; i < pDraw->pScreen->numDepths; i++, pDepth++)
            if (pDepth->depth == stuff->depth)
                goto CreatePmap;
        client->errorValue = stuff->depth;
        return BadValue;
    }

 CreatePmap:
    size = PixmapBytePad(width, depth) * height;
    if (sizeof(size) == 4 && BitsPerPixel(depth) > 8) {
        if (size < width * height)
            return BadAlloc;
    }
     
    if (stuff->offset + size < size)
        return BadAlloc;

    VERIFY_SHMSIZE(shmdesc, stuff->offset, size, client);

    if (!(newPix = malloc(sizeof(PanoramiXRes))))
        return BadAlloc;

    newPix->type = XRT_PIXMAP;
    newPix->u.pix.shared = TRUE;
    panoramix_setup_ids(newPix, client, stuff->pid);

    result = Success;

    FOR_NSCREENS(j) {
        ShmScrPrivateRec *screen_priv;

        pScreen = screenInfo.screens[j];

        screen_priv = ShmGetScreenPriv(pScreen);
        pMap = (*screen_priv->shmFuncs->CreatePixmap) (pScreen,
                                                       stuff->width,
                                                       stuff->height,
                                                       stuff->depth,
                                                       shmdesc->addr +
                                                       stuff->offset);

        if (pMap) {
            result = XaceHook(XACE_RESOURCE_ACCESS, client, stuff->pid,
                              RT_PIXMAP, pMap, RT_NONE, NULL, DixCreateAccess);
            if (result != Success) {
                pDraw->pScreen->DestroyPixmap(pMap);
                break;
            }
            dixSetPrivate(&pMap->devPrivates, shmPixmapPrivateKey, shmdesc);
            shmdesc->refcnt++;
            pMap->drawable.serialNumber = NEXT_SERIAL_NUMBER;
            pMap->drawable.id = newPix->info[j].id;
            if (!AddResource(newPix->info[j].id, RT_PIXMAP, (void *) pMap)) {
                result = BadAlloc;
                break;
            }
        }
        else {
            result = BadAlloc;
            break;
        }
    }

    if (result != Success) {
        while (j--)
            FreeResource(newPix->info[j].id, RT_NONE);
        free(newPix);
    }
    else
        AddResource(stuff->pid, XRT_PIXMAP, newPix);

    return result;
}
