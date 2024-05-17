ProcDbeAllocateBackBufferName(ClientPtr client)
{
    REQUEST(xDbeAllocateBackBufferNameReq);
    WindowPtr pWin;
    DbeScreenPrivPtr pDbeScreenPriv;
    DbeWindowPrivPtr pDbeWindowPriv;
    XdbeScreenVisualInfo scrVisInfo;
    register int i;
    Bool visualMatched = FALSE;
    xDbeSwapAction swapAction;
    VisualID visual;
    int status;
    int add_index;

    REQUEST_SIZE_MATCH(xDbeAllocateBackBufferNameReq);

     
    status = dixLookupWindow(&pWin, stuff->window, client, DixManageAccess);
    if (status != Success)
        return status;

     
    if (pWin->drawable.class != InputOutput) {
        return BadMatch;
    }

     
    swapAction = stuff->swapAction;      
    if ((swapAction != XdbeUndefined) &&
        (swapAction != XdbeBackground) &&
        (swapAction != XdbeUntouched) && (swapAction != XdbeCopied)) {
        return BadValue;
    }

     
    LEGAL_NEW_RESOURCE(stuff->buffer, client);

     
    pDbeScreenPriv = DBE_SCREEN_PRIV_FROM_WINDOW(pWin);
    if (!pDbeScreenPriv->GetVisualInfo)
        return BadMatch;         

    if (!(*pDbeScreenPriv->GetVisualInfo) (pWin->drawable.pScreen, &scrVisInfo)) {
         
        return BadAlloc;
    }

     
    visual = wVisual(pWin);
    for (i = 0; (i < scrVisInfo.count) && !visualMatched; i++) {
        if (scrVisInfo.visinfo[i].visual == visual) {
            visualMatched = TRUE;
        }
    }

     
    free(scrVisInfo.visinfo);

    if (!visualMatched) {
        return BadMatch;
    }

    if ((pDbeWindowPriv = DBE_WINDOW_PRIV(pWin)) == NULL) {
         

        pDbeWindowPriv = calloc(1, sizeof(DbeWindowPrivRec));
        if (!pDbeWindowPriv)
            return BadAlloc;

         
        pDbeWindowPriv->pWindow = pWin;
        pDbeWindowPriv->width = pWin->drawable.width;
        pDbeWindowPriv->height = pWin->drawable.height;
        pDbeWindowPriv->x = pWin->drawable.x;
        pDbeWindowPriv->y = pWin->drawable.y;
        pDbeWindowPriv->nBufferIDs = 0;

         
        pDbeWindowPriv->IDs = pDbeWindowPriv->initIDs;

         
        pDbeWindowPriv->maxAvailableIDs = DBE_INIT_MAX_IDS;
        pDbeWindowPriv->IDs[0] = stuff->buffer;

        add_index = 0;
        for (i = 0; i < DBE_INIT_MAX_IDS; i++) {
            pDbeWindowPriv->IDs[i] = DBE_FREE_ID_ELEMENT;
        }

         
        dixSetPrivate(&pWin->devPrivates, dbeWindowPrivKey, pDbeWindowPriv);

    }                            

    else {
         

         
        for (i = 0; i < pDbeWindowPriv->maxAvailableIDs; i++) {
            if (pDbeWindowPriv->IDs[i] == DBE_FREE_ID_ELEMENT) {
                 
                break;
            }
        }

        if (i == pDbeWindowPriv->maxAvailableIDs) {
             
            XID *pIDs;

             
            if (pDbeWindowPriv->maxAvailableIDs == DBE_INIT_MAX_IDS) {
                 
                pIDs = NULL;
            }
            else {
                 
                pIDs = pDbeWindowPriv->IDs;
            }

             
            pDbeWindowPriv->IDs =
                reallocarray(pIDs,
                             pDbeWindowPriv->maxAvailableIDs + DBE_INCR_MAX_IDS,
                             sizeof(XID));
            if (!pDbeWindowPriv->IDs) {
                return BadAlloc;
            }
            memset(&pDbeWindowPriv->IDs[pDbeWindowPriv->nBufferIDs], 0,
                   (pDbeWindowPriv->maxAvailableIDs + DBE_INCR_MAX_IDS -
                    pDbeWindowPriv->nBufferIDs) * sizeof(XID));

            if (pDbeWindowPriv->maxAvailableIDs == DBE_INIT_MAX_IDS) {
                 
                memcpy(pDbeWindowPriv->IDs, pDbeWindowPriv->initIDs,
                       DBE_INIT_MAX_IDS * sizeof(XID));
            }

            pDbeWindowPriv->maxAvailableIDs += DBE_INCR_MAX_IDS;
        }

        add_index = i;

    }                            

     
    status = (*pDbeScreenPriv->AllocBackBufferName) (pWin, stuff->buffer,
                                                     stuff->swapAction);

    if (status == Success) {
        pDbeWindowPriv->IDs[add_index] = stuff->buffer;
        if (!AddResource(stuff->buffer, dbeWindowPrivResType,
                         (void *) pDbeWindowPriv)) {
            pDbeWindowPriv->IDs[add_index] = DBE_FREE_ID_ELEMENT;

            if (pDbeWindowPriv->nBufferIDs == 0) {
                status = BadAlloc;
                goto out_free;
            }
        }
    }
    else {
         
        if (pDbeWindowPriv->nBufferIDs == 0) {
            goto out_free;
        }
    }

     
    pDbeWindowPriv->nBufferIDs++;

     
    pDbeWindowPriv->swapAction = stuff->swapAction;

    return status;

 out_free:
    dixSetPrivate(&pWin->devPrivates, dbeWindowPrivKey, NULL);
    free(pDbeWindowPriv);
    return status;

}                                
