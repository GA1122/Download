PanoramiXTranslateVisualID(int screen, VisualID orig)
{
    ScreenPtr pOtherScreen = screenInfo.screens[screen];
    VisualPtr pVisual = NULL;
    int i;

    for (i = 0; i < PanoramiXNumVisuals; i++) {
        if (orig == PanoramiXVisuals[i].vid) {
            pVisual = &PanoramiXVisuals[i];
            break;
        }
    }

    if (!pVisual)
        return 0;

     
    if (screen == 0)
        return orig;

     
    for (i = 0; i < pOtherScreen->numVisuals; i++) {
        VisualPtr pOtherVisual = &pOtherScreen->visuals[i];

        if ((*XineramaVisualsEqualPtr) (pVisual, pOtherScreen, pOtherVisual))
            return pOtherVisual->vid;
    }

    return 0;
}
