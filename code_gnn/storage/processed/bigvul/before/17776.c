ProcDbeSwapBuffers(ClientPtr client)
{
    REQUEST(xDbeSwapBuffersReq);
    WindowPtr pWin;
    DbeScreenPrivPtr pDbeScreenPriv;
    DbeSwapInfoPtr swapInfo;
    xDbeSwapInfo *dbeSwapInfo;
    int error;
    unsigned int i, j;
    unsigned int nStuff;
    int nStuff_i;        

    REQUEST_AT_LEAST_SIZE(xDbeSwapBuffersReq);
    nStuff = stuff->n;           

    if (nStuff == 0) {
        REQUEST_SIZE_MATCH(xDbeSwapBuffersReq);
        return Success;
    }

    if (nStuff > UINT32_MAX / sizeof(DbeSwapInfoRec))
        return BadAlloc;
    REQUEST_FIXED_SIZE(xDbeSwapBuffersReq, nStuff * sizeof(xDbeSwapInfo));

     
    dbeSwapInfo = (xDbeSwapInfo *) &stuff[1];

     
    swapInfo = xallocarray(nStuff, sizeof(DbeSwapInfoRec));
    if (swapInfo == NULL) {
        return BadAlloc;
    }

    for (i = 0; i < nStuff; i++) {
         

         
        error = dixLookupWindow(&pWin, dbeSwapInfo[i].window, client,
                                DixWriteAccess);
        if (error != Success) {
            free(swapInfo);
            return error;
        }

         
        if (DBE_WINDOW_PRIV(pWin) == NULL) {
            free(swapInfo);
            return BadMatch;
        }

         
        for (j = i + 1; j < nStuff; j++) {
            if (dbeSwapInfo[i].window == dbeSwapInfo[j].window) {
                free(swapInfo);
                return BadMatch;
            }
        }

         
        if ((dbeSwapInfo[i].swapAction != XdbeUndefined) &&
            (dbeSwapInfo[i].swapAction != XdbeBackground) &&
            (dbeSwapInfo[i].swapAction != XdbeUntouched) &&
            (dbeSwapInfo[i].swapAction != XdbeCopied)) {
            free(swapInfo);
            return BadValue;
        }

         
        swapInfo[i].pWindow = pWin;
        swapInfo[i].swapAction = dbeSwapInfo[i].swapAction;

    }                            

     

    nStuff_i = nStuff;
    while (nStuff_i > 0) {
        pDbeScreenPriv = DBE_SCREEN_PRIV_FROM_WINDOW(swapInfo[0].pWindow);
        error = (*pDbeScreenPriv->SwapBuffers) (client, &nStuff_i, swapInfo);
        if (error != Success) {
            free(swapInfo);
            return error;
        }
    }

    free(swapInfo);
    return Success;

}                                
