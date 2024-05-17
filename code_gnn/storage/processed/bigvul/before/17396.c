XineramaValidateGC(GCPtr pGC, unsigned long changes, DrawablePtr pDraw)
{
    Xinerama_GC_FUNC_PROLOGUE(pGC);

    if ((pDraw->type == DRAWABLE_WINDOW) && !(((WindowPtr) pDraw)->parent)) {
         
        int x_off = pGC->pScreen->x;
        int y_off = pGC->pScreen->y;
        int new_val;

        new_val = pGCPriv->clipOrg.x - x_off;
        if (pGC->clipOrg.x != new_val) {
            pGC->clipOrg.x = new_val;
            changes |= GCClipXOrigin;
        }
        new_val = pGCPriv->clipOrg.y - y_off;
        if (pGC->clipOrg.y != new_val) {
            pGC->clipOrg.y = new_val;
            changes |= GCClipYOrigin;
        }
        new_val = pGCPriv->patOrg.x - x_off;
        if (pGC->patOrg.x != new_val) {
            pGC->patOrg.x = new_val;
            changes |= GCTileStipXOrigin;
        }
        new_val = pGCPriv->patOrg.y - y_off;
        if (pGC->patOrg.y != new_val) {
            pGC->patOrg.y = new_val;
            changes |= GCTileStipYOrigin;
        }
    }
    else {
        if (pGC->clipOrg.x != pGCPriv->clipOrg.x) {
            pGC->clipOrg.x = pGCPriv->clipOrg.x;
            changes |= GCClipXOrigin;
        }
        if (pGC->clipOrg.y != pGCPriv->clipOrg.y) {
            pGC->clipOrg.y = pGCPriv->clipOrg.y;
            changes |= GCClipYOrigin;
        }
        if (pGC->patOrg.x != pGCPriv->patOrg.x) {
            pGC->patOrg.x = pGCPriv->patOrg.x;
            changes |= GCTileStipXOrigin;
        }
        if (pGC->patOrg.y != pGCPriv->patOrg.y) {
            pGC->patOrg.y = pGCPriv->patOrg.y;
            changes |= GCTileStipYOrigin;
        }
    }

    (*pGC->funcs->ValidateGC) (pGC, changes, pDraw);
    Xinerama_GC_FUNC_EPILOGUE(pGC);
}
