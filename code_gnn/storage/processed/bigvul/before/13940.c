gs_currentoverrideicc(const gs_gstate *pgs)
{
    if (pgs->icc_manager != NULL) {
        return pgs->icc_manager->override_internal;
    } else {
        return false;
    }
}
