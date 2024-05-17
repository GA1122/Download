PanoramiXMaybeAddVisual(VisualPtr pVisual)
{
    ScreenPtr pScreen;
    int j, k;
    Bool found = FALSE;

    FOR_NSCREENS_FORWARD_SKIP(j) {
        pScreen = screenInfo.screens[j];
        found = FALSE;

        for (k = 0; k < pScreen->numVisuals; k++) {
            VisualPtr candidate = &pScreen->visuals[k];

            if ((*XineramaVisualsEqualPtr) (pVisual, pScreen, candidate)
#ifdef GLXPROXY
                && glxMatchVisual(screenInfo.screens[0], pVisual, pScreen)
#endif
                ) {
                found = TRUE;
                break;
            }
        }

        if (!found)
            return;
    }

     
    j = PanoramiXNumVisuals;
    PanoramiXNumVisuals++;
    PanoramiXVisuals = reallocarray(PanoramiXVisuals,
                                    PanoramiXNumVisuals, sizeof(VisualRec));

    memcpy(&PanoramiXVisuals[j], pVisual, sizeof(VisualRec));

    for (k = 0; k < PanoramiXNumDepths; k++) {
        if (PanoramiXDepths[k].depth == pVisual->nplanes) {
            PanoramiXDepths[k].vids[PanoramiXDepths[k].numVids] = pVisual->vid;
            PanoramiXDepths[k].numVids++;
            break;
        }
    }
}
