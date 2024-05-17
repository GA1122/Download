DGADestroyColormap(ColormapPtr pmap)
{
    ScreenPtr pScreen = pmap->pScreen;
    DGAScreenPtr pScreenPriv = DGA_GET_SCREEN_PRIV(pScreen);
    VisualPtr pVisual = pmap->pVisual;

    if (pScreenPriv->fakedVisuals) {
        FakedVisualList *curr = pScreenPriv->fakedVisuals;

        while (curr) {
            if (curr->pVisual == pVisual) {
                 
                curr->free = TRUE;
                break;
            }
            curr = curr->next;
        }
    }

    if (pScreenPriv->DestroyColormap) {
        pScreen->DestroyColormap = pScreenPriv->DestroyColormap;
        (*pScreen->DestroyColormap) (pmap);
        pScreen->DestroyColormap = DGADestroyColormap;
    }
}
