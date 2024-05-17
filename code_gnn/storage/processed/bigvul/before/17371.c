PanoramiXMaybeAddDepth(DepthPtr pDepth)
{
    ScreenPtr pScreen;
    int j, k;
    Bool found = FALSE;

    FOR_NSCREENS_FORWARD_SKIP(j) {
        pScreen = screenInfo.screens[j];
        for (k = 0; k < pScreen->numDepths; k++) {
            if (pScreen->allowedDepths[k].depth == pDepth->depth) {
                found = TRUE;
                break;
            }
        }
    }

    if (!found)
        return;

    j = PanoramiXNumDepths;
    PanoramiXNumDepths++;
    PanoramiXDepths = reallocarray(PanoramiXDepths,
                                   PanoramiXNumDepths, sizeof(DepthRec));
    PanoramiXDepths[j].depth = pDepth->depth;
    PanoramiXDepths[j].numVids = 0;
     
    if (pDepth->numVids)
        PanoramiXDepths[j].vids = xallocarray(pDepth->numVids, sizeof(VisualID));
    else
        PanoramiXDepths[j].vids = NULL;
}
