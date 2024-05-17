gs_setoverrideicc(gs_gstate *pgs, bool value)
{
    if (pgs->icc_manager != NULL) {
        pgs->icc_manager->override_internal = value;
    }
}
