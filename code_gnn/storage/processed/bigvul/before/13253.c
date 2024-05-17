c_pdf14trans_adjust_ctm(gs_composite_t * pct0, int x0, int y0, gs_gstate *pgs)
{
    gs_pdf14trans_t *pct = (gs_pdf14trans_t *)pct0;
    gs_matrix mat = pct->params.ctm;

    if_debug6m('L', pgs->memory, " [%g %g %g %g %g %g]\n",
               mat.xx, mat.xy, mat.yx, mat.yy,
               mat.tx, mat.ty);
    mat.tx -= x0;
    mat.ty -= y0;
    gs_gstate_setmatrix(pgs, &mat);
    return 0;
}
