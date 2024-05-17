ProcXF86DRIGetDrawableInfo(register ClientPtr client)
{
    xXF86DRIGetDrawableInfoReply rep = {
        .type = X_Reply,
        .sequenceNumber = client->sequence,
        .length = 0
    };
    DrawablePtr pDrawable;
    int X, Y, W, H;
    drm_clip_rect_t *pClipRects, *pClippedRects;
    drm_clip_rect_t *pBackClipRects;
    int backX, backY, rc;

    REQUEST(xXF86DRIGetDrawableInfoReq);
    REQUEST_SIZE_MATCH(xXF86DRIGetDrawableInfoReq);
    if (stuff->screen >= screenInfo.numScreens) {
        client->errorValue = stuff->screen;
        return BadValue;
    }

    rc = dixLookupDrawable(&pDrawable, stuff->drawable, client, 0,
                           DixReadAccess);
    if (rc != Success)
        return rc;

    if (!DRIGetDrawableInfo(screenInfo.screens[stuff->screen],
                            pDrawable,
                            (unsigned int *) &rep.drawableTableIndex,
                            (unsigned int *) &rep.drawableTableStamp,
                            (int *) &X,
                            (int *) &Y,
                            (int *) &W,
                            (int *) &H,
                            (int *) &rep.numClipRects,
                            &pClipRects,
                            &backX,
                            &backY,
                            (int *) &rep.numBackClipRects, &pBackClipRects)) {
        return BadValue;
    }

    rep.drawableX = X;
    rep.drawableY = Y;
    rep.drawableWidth = W;
    rep.drawableHeight = H;
    rep.length = (SIZEOF(xXF86DRIGetDrawableInfoReply) - SIZEOF(xGenericReply));

    rep.backX = backX;
    rep.backY = backY;

    if (rep.numBackClipRects)
        rep.length += sizeof(drm_clip_rect_t) * rep.numBackClipRects;

    pClippedRects = pClipRects;

    if (rep.numClipRects) {
         
        pClippedRects = xallocarray(rep.numClipRects, sizeof(drm_clip_rect_t));

        if (pClippedRects) {
            ScreenPtr pScreen = screenInfo.screens[stuff->screen];
            int i, j;

            for (i = 0, j = 0; i < rep.numClipRects; i++) {
                pClippedRects[j].x1 = max(pClipRects[i].x1, 0);
                pClippedRects[j].y1 = max(pClipRects[i].y1, 0);
                pClippedRects[j].x2 = min(pClipRects[i].x2, pScreen->width);
                pClippedRects[j].y2 = min(pClipRects[i].y2, pScreen->height);

                if (pClippedRects[j].x1 < pClippedRects[j].x2 &&
                    pClippedRects[j].y1 < pClippedRects[j].y2) {
                    j++;
                }
            }

            rep.numClipRects = j;
        }
        else {
            rep.numClipRects = 0;
        }

        rep.length += sizeof(drm_clip_rect_t) * rep.numClipRects;
    }

    rep.length = bytes_to_int32(rep.length);

    WriteToClient(client, sizeof(xXF86DRIGetDrawableInfoReply), &rep);

    if (rep.numClipRects) {
        WriteToClient(client,
                      sizeof(drm_clip_rect_t) * rep.numClipRects,
                      pClippedRects);
        free(pClippedRects);
    }

    if (rep.numBackClipRects) {
        WriteToClient(client,
                      sizeof(drm_clip_rect_t) * rep.numBackClipRects,
                      pBackClipRects);
    }

    return Success;
}
