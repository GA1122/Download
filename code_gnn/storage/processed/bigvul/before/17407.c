ProcScreenSaverSetAttributes(ClientPtr client)
{
#ifdef PANORAMIX
    if (!noPanoramiXExtension) {
        REQUEST(xScreenSaverSetAttributesReq);
        PanoramiXRes *draw;
        PanoramiXRes *backPix = NULL;
        PanoramiXRes *bordPix = NULL;
        PanoramiXRes *cmap = NULL;
        int i, status, len;
        int pback_offset = 0, pbord_offset = 0, cmap_offset = 0;
        XID orig_visual, tmp;

        REQUEST_AT_LEAST_SIZE(xScreenSaverSetAttributesReq);

        status = dixLookupResourceByClass((void **) &draw, stuff->drawable,
                                          XRC_DRAWABLE, client, DixWriteAccess);
        if (status != Success)
            return (status == BadValue) ? BadDrawable : status;

        len =
            stuff->length -
            bytes_to_int32(sizeof(xScreenSaverSetAttributesReq));
        if (Ones(stuff->mask) != len)
            return BadLength;

        if ((Mask) stuff->mask & CWBackPixmap) {
            pback_offset = Ones((Mask) stuff->mask & (CWBackPixmap - 1));
            tmp = *((CARD32 *) &stuff[1] + pback_offset);
            if ((tmp != None) && (tmp != ParentRelative)) {
                status = dixLookupResourceByType((void **) &backPix, tmp,
                                                 XRT_PIXMAP, client,
                                                 DixReadAccess);
                if (status != Success)
                    return status;
            }
        }

        if ((Mask) stuff->mask & CWBorderPixmap) {
            pbord_offset = Ones((Mask) stuff->mask & (CWBorderPixmap - 1));
            tmp = *((CARD32 *) &stuff[1] + pbord_offset);
            if (tmp != CopyFromParent) {
                status = dixLookupResourceByType((void **) &bordPix, tmp,
                                                 XRT_PIXMAP, client,
                                                 DixReadAccess);
                if (status != Success)
                    return status;
            }
        }

        if ((Mask) stuff->mask & CWColormap) {
            cmap_offset = Ones((Mask) stuff->mask & (CWColormap - 1));
            tmp = *((CARD32 *) &stuff[1] + cmap_offset);
            if (tmp != CopyFromParent) {
                status = dixLookupResourceByType((void **) &cmap, tmp,
                                                 XRT_COLORMAP, client,
                                                 DixReadAccess);
                if (status != Success)
                    return status;
            }
        }

        orig_visual = stuff->visualID;

        FOR_NSCREENS_BACKWARD(i) {
            stuff->drawable = draw->info[i].id;
            if (backPix)
                *((CARD32 *) &stuff[1] + pback_offset) = backPix->info[i].id;
            if (bordPix)
                *((CARD32 *) &stuff[1] + pbord_offset) = bordPix->info[i].id;
            if (cmap)
                *((CARD32 *) &stuff[1] + cmap_offset) = cmap->info[i].id;

            if (orig_visual != CopyFromParent)
                stuff->visualID = PanoramiXTranslateVisualID(i, orig_visual);

            status = ScreenSaverSetAttributes(client);
        }

        return status;
    }
#endif

    return ScreenSaverSetAttributes(client);
}
