XineramaChangeClip(GCPtr pGC, int type, void *pvalue, int nrects)
{
    Xinerama_GC_FUNC_PROLOGUE(pGC);
    (*pGC->funcs->ChangeClip) (pGC, type, pvalue, nrects);
    Xinerama_GC_FUNC_EPILOGUE(pGC);
}
