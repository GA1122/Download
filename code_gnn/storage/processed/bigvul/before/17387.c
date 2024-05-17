XineramaCreateGC(GCPtr pGC)
{
    ScreenPtr pScreen = pGC->pScreen;
    PanoramiXScreenPtr pScreenPriv = (PanoramiXScreenPtr)
        dixLookupPrivate(&pScreen->devPrivates, PanoramiXScreenKey);
    Bool ret;

    pScreen->CreateGC = pScreenPriv->CreateGC;
    if ((ret = (*pScreen->CreateGC) (pGC))) {
        PanoramiXGCPtr pGCPriv = (PanoramiXGCPtr)
            dixLookupPrivate(&pGC->devPrivates, PanoramiXGCKey);

        pGCPriv->wrapFuncs = pGC->funcs;
        pGC->funcs = &XineramaGCFuncs;

        pGCPriv->clipOrg.x = pGC->clipOrg.x;
        pGCPriv->clipOrg.y = pGC->clipOrg.y;
        pGCPriv->patOrg.x = pGC->patOrg.x;
        pGCPriv->patOrg.y = pGC->patOrg.y;
    }
    pScreen->CreateGC = XineramaCreateGC;

    return ret;
}
