FreeMarkedVisuals(ScreenPtr pScreen)
{
    DGAScreenPtr pScreenPriv = DGA_GET_SCREEN_PRIV(pScreen);
    FakedVisualList *prev, *curr, *tmp;

    if (!pScreenPriv->fakedVisuals)
        return;

    prev = NULL;
    curr = pScreenPriv->fakedVisuals;

    while (curr) {
        if (curr->free) {
            tmp = curr;
            curr = curr->next;
            if (prev)
                prev->next = curr;
            else
                pScreenPriv->fakedVisuals = curr;
            free(tmp->pVisual);
            free(tmp);
        }
        else {
            prev = curr;
            curr = curr->next;
        }
    }
}
