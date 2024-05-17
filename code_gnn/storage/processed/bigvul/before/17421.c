ScreenSaverSetAttributes(ClientPtr client)
{
    REQUEST(xScreenSaverSetAttributesReq);
    DrawablePtr pDraw;
    WindowPtr pParent;
    ScreenPtr pScreen;
    ScreenSaverScreenPrivatePtr pPriv = 0;
    ScreenSaverAttrPtr pAttr = 0;
    int ret, len, class, bw, depth;
    unsigned long visual;
    int idepth, ivisual;
    Bool fOK;
    DepthPtr pDepth;
    WindowOptPtr ancwopt;
    unsigned int *pVlist;
    unsigned long *values = 0;
    unsigned long tmask, imask;
    unsigned long val;
    Pixmap pixID;
    PixmapPtr pPixmap;
    Cursor cursorID;
    CursorPtr pCursor;
    Colormap cmap;
    ColormapPtr pCmap;

    REQUEST_AT_LEAST_SIZE(xScreenSaverSetAttributesReq);
    ret = dixLookupDrawable(&pDraw, stuff->drawable, client, 0,
                            DixGetAttrAccess);
    if (ret != Success)
        return ret;
    pScreen = pDraw->pScreen;
    pParent = pScreen->root;

    ret = XaceHook(XACE_SCREENSAVER_ACCESS, client, pScreen, DixSetAttrAccess);
    if (ret != Success)
        return ret;

    len = stuff->length - bytes_to_int32(sizeof(xScreenSaverSetAttributesReq));
    if (Ones(stuff->mask) != len)
        return BadLength;
    if (!stuff->width || !stuff->height) {
        client->errorValue = 0;
        return BadValue;
    }
    switch (class = stuff->c_class) {
    case CopyFromParent:
    case InputOnly:
    case InputOutput:
        break;
    default:
        client->errorValue = class;
        return BadValue;
    }
    bw = stuff->borderWidth;
    depth = stuff->depth;
    visual = stuff->visualID;

     

    if (class == CopyFromParent)
        class = pParent->drawable.class;

    if ((class != InputOutput) && (class != InputOnly)) {
        client->errorValue = class;
        return BadValue;
    }

    if ((class != InputOnly) && (pParent->drawable.class == InputOnly))
        return BadMatch;

    if ((class == InputOnly) && ((bw != 0) || (depth != 0)))
        return BadMatch;

    if ((class == InputOutput) && (depth == 0))
        depth = pParent->drawable.depth;
    ancwopt = pParent->optional;
    if (!ancwopt)
        ancwopt = FindWindowWithOptional(pParent)->optional;
    if (visual == CopyFromParent)
        visual = ancwopt->visual;

     
    if ((visual != ancwopt->visual) || (depth != pParent->drawable.depth)) {
        fOK = FALSE;
        for (idepth = 0; idepth < pScreen->numDepths; idepth++) {
            pDepth = (DepthPtr) &pScreen->allowedDepths[idepth];
            if ((depth == pDepth->depth) || (depth == 0)) {
                for (ivisual = 0; ivisual < pDepth->numVids; ivisual++) {
                    if (visual == pDepth->vids[ivisual]) {
                        fOK = TRUE;
                        break;
                    }
                }
            }
        }
        if (fOK == FALSE)
            return BadMatch;
    }

    if (((stuff->mask & (CWBorderPixmap | CWBorderPixel)) == 0) &&
        (class != InputOnly) && (depth != pParent->drawable.depth)) {
        return BadMatch;
    }

    if (((stuff->mask & CWColormap) == 0) &&
        (class != InputOnly) &&
        ((visual != ancwopt->visual) || (ancwopt->colormap == None))) {
        return BadMatch;
    }

     

    pPriv = GetScreenPrivate(pScreen);
    if (pPriv && pPriv->attr) {
        if (pPriv->attr->client != client)
            return BadAccess;
    }
    if (!pPriv) {
        pPriv = MakeScreenPrivate(pScreen);
        if (!pPriv)
            return FALSE;
    }
    pAttr = New(ScreenSaverAttrRec);
    if (!pAttr) {
        ret = BadAlloc;
        goto bail;
    }
     
    pAttr->values = values = xallocarray(len + 1, sizeof(unsigned long));
    if (!values) {
        ret = BadAlloc;
        goto bail;
    }
    pAttr->screen = pScreen;
    pAttr->client = client;
    pAttr->x = stuff->x;
    pAttr->y = stuff->y;
    pAttr->width = stuff->width;
    pAttr->height = stuff->height;
    pAttr->borderWidth = stuff->borderWidth;
    pAttr->class = stuff->c_class;
    pAttr->depth = depth;
    pAttr->visual = visual;
    pAttr->colormap = None;
    pAttr->pCursor = NullCursor;
    pAttr->pBackgroundPixmap = NullPixmap;
    pAttr->pBorderPixmap = NullPixmap;
     
    pAttr->mask = tmask = stuff->mask | CWOverrideRedirect;
    pVlist = (unsigned int *) (stuff + 1);
    while (tmask) {
        imask = lowbit(tmask);
        tmask &= ~imask;
        switch (imask) {
        case CWBackPixmap:
            pixID = (Pixmap) * pVlist;
            if (pixID == None) {
                *values++ = None;
            }
            else if (pixID == ParentRelative) {
                if (depth != pParent->drawable.depth) {
                    ret = BadMatch;
                    goto PatchUp;
                }
                *values++ = ParentRelative;
            }
            else {
                ret =
                    dixLookupResourceByType((void **) &pPixmap, pixID,
                                            RT_PIXMAP, client, DixReadAccess);
                if (ret == Success) {
                    if ((pPixmap->drawable.depth != depth) ||
                        (pPixmap->drawable.pScreen != pScreen)) {
                        ret = BadMatch;
                        goto PatchUp;
                    }
                    pAttr->pBackgroundPixmap = pPixmap;
                    pPixmap->refcnt++;
                    pAttr->mask &= ~CWBackPixmap;
                }
                else {
                    client->errorValue = pixID;
                    goto PatchUp;
                }
            }
            break;
        case CWBackPixel:
            *values++ = (CARD32) *pVlist;
            break;
        case CWBorderPixmap:
            pixID = (Pixmap) * pVlist;
            if (pixID == CopyFromParent) {
                if (depth != pParent->drawable.depth) {
                    ret = BadMatch;
                    goto PatchUp;
                }
                *values++ = CopyFromParent;
            }
            else {
                ret =
                    dixLookupResourceByType((void **) &pPixmap, pixID,
                                            RT_PIXMAP, client, DixReadAccess);
                if (ret == Success) {
                    if ((pPixmap->drawable.depth != depth) ||
                        (pPixmap->drawable.pScreen != pScreen)) {
                        ret = BadMatch;
                        goto PatchUp;
                    }
                    pAttr->pBorderPixmap = pPixmap;
                    pPixmap->refcnt++;
                    pAttr->mask &= ~CWBorderPixmap;
                }
                else {
                    client->errorValue = pixID;
                    goto PatchUp;
                }
            }
            break;
        case CWBorderPixel:
            *values++ = (CARD32) *pVlist;
            break;
        case CWBitGravity:
            val = (CARD8) *pVlist;
            if (val > StaticGravity) {
                ret = BadValue;
                client->errorValue = val;
                goto PatchUp;
            }
            *values++ = val;
            break;
        case CWWinGravity:
            val = (CARD8) *pVlist;
            if (val > StaticGravity) {
                ret = BadValue;
                client->errorValue = val;
                goto PatchUp;
            }
            *values++ = val;
            break;
        case CWBackingStore:
            val = (CARD8) *pVlist;
            if ((val != NotUseful) && (val != WhenMapped) && (val != Always)) {
                ret = BadValue;
                client->errorValue = val;
                goto PatchUp;
            }
            *values++ = val;
            break;
        case CWBackingPlanes:
            *values++ = (CARD32) *pVlist;
            break;
        case CWBackingPixel:
            *values++ = (CARD32) *pVlist;
            break;
        case CWSaveUnder:
            val = (BOOL) * pVlist;
            if ((val != xTrue) && (val != xFalse)) {
                ret = BadValue;
                client->errorValue = val;
                goto PatchUp;
            }
            *values++ = val;
            break;
        case CWEventMask:
            *values++ = (CARD32) *pVlist;
            break;
        case CWDontPropagate:
            *values++ = (CARD32) *pVlist;
            break;
        case CWOverrideRedirect:
            if (!(stuff->mask & CWOverrideRedirect))
                pVlist--;
            else {
                val = (BOOL) * pVlist;
                if ((val != xTrue) && (val != xFalse)) {
                    ret = BadValue;
                    client->errorValue = val;
                    goto PatchUp;
                }
            }
            *values++ = xTrue;
            break;
        case CWColormap:
            cmap = (Colormap) * pVlist;
            ret = dixLookupResourceByType((void **) &pCmap, cmap, RT_COLORMAP,
                                          client, DixUseAccess);
            if (ret != Success) {
                client->errorValue = cmap;
                goto PatchUp;
            }
            if (pCmap->pVisual->vid != visual || pCmap->pScreen != pScreen) {
                ret = BadMatch;
                goto PatchUp;
            }
            pAttr->colormap = cmap;
            pAttr->mask &= ~CWColormap;
            break;
        case CWCursor:
            cursorID = (Cursor) * pVlist;
            if (cursorID == None) {
                *values++ = None;
            }
            else {
                ret = dixLookupResourceByType((void **) &pCursor, cursorID,
                                              RT_CURSOR, client, DixUseAccess);
                if (ret != Success) {
                    client->errorValue = cursorID;
                    goto PatchUp;
                }
                pAttr->pCursor = RefCursor(pCursor);
                pAttr->mask &= ~CWCursor;
            }
            break;
        default:
            ret = BadValue;
            client->errorValue = stuff->mask;
            goto PatchUp;
        }
        pVlist++;
    }
    if (pPriv->attr)
        FreeScreenAttr(pPriv->attr);
    pPriv->attr = pAttr;
    pAttr->resource = FakeClientID(client->index);
    if (!AddResource(pAttr->resource, AttrType, (void *) pAttr))
        return BadAlloc;
    return Success;
 PatchUp:
    FreeAttrs(pAttr);
 bail:
    CheckScreenPrivate(pScreen);
    if (pAttr)
        free(pAttr->values);
    free(pAttr);
    return ret;
}
