XineramaChangeGC(GCPtr pGC, unsigned long mask)
{
    Xinerama_GC_FUNC_PROLOGUE(pGC);

    if (mask & GCTileStipXOrigin)
        pGCPriv->patOrg.x = pGC->patOrg.x;
    if (mask & GCTileStipYOrigin)
        pGCPriv->patOrg.y = pGC->patOrg.y;
    if (mask & GCClipXOrigin)
        pGCPriv->clipOrg.x = pGC->clipOrg.x;
    if (mask & GCClipYOrigin)
        pGCPriv->clipOrg.y = pGC->clipOrg.y;

    (*pGC->funcs->ChangeGC) (pGC, mask);
    Xinerama_GC_FUNC_EPILOGUE(pGC);
}
