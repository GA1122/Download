XineramaInitData(void)
{
    int i, w, h;

    RegionNull(&PanoramiXScreenRegion);
    FOR_NSCREENS(i) {
        BoxRec TheBox;
        RegionRec ScreenRegion;

        ScreenPtr pScreen = screenInfo.screens[i];

        TheBox.x1 = pScreen->x;
        TheBox.x2 = TheBox.x1 + pScreen->width;
        TheBox.y1 = pScreen->y;
        TheBox.y2 = TheBox.y1 + pScreen->height;

        RegionInit(&ScreenRegion, &TheBox, 1);
        RegionUnion(&PanoramiXScreenRegion, &PanoramiXScreenRegion,
                    &ScreenRegion);
        RegionUninit(&ScreenRegion);
    }

    PanoramiXPixWidth = screenInfo.screens[0]->x + screenInfo.screens[0]->width;
    PanoramiXPixHeight =
        screenInfo.screens[0]->y + screenInfo.screens[0]->height;

    FOR_NSCREENS_FORWARD_SKIP(i) {
        ScreenPtr pScreen = screenInfo.screens[i];

        w = pScreen->x + pScreen->width;
        h = pScreen->y + pScreen->height;

        if (PanoramiXPixWidth < w)
            PanoramiXPixWidth = w;
        if (PanoramiXPixHeight < h)
            PanoramiXPixHeight = h;
    }
}
