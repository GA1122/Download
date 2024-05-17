XineramaCloseScreen(ScreenPtr pScreen)
{
    PanoramiXScreenPtr pScreenPriv = (PanoramiXScreenPtr)
        dixLookupPrivate(&pScreen->devPrivates, PanoramiXScreenKey);

    pScreen->CloseScreen = pScreenPriv->CloseScreen;
    pScreen->CreateGC = pScreenPriv->CreateGC;

    if (pScreen->myNum == 0)
        RegionUninit(&PanoramiXScreenRegion);

    free(pScreenPriv);

    return (*pScreen->CloseScreen) (pScreen);
}
