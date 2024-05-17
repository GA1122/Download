z2restore(i_ctx_t *i_ctx_p)
{
    alloc_save_t *asave;
    bool saveLockSafety = gs_currentdevice_inline(igs)->LockSafetyParams;
    int code = restore_check_save(i_ctx_p, &asave);

    if (code < 0) return code;

    while (gs_gstate_saved(gs_gstate_saved(igs))) {
        code = restore_page_device(i_ctx_p, igs, gs_gstate_saved(igs));
        if (code < 0) return code;
        if (code > 0)
            return push_callout(i_ctx_p, "%restore1pagedevice");
        gs_grestore(igs);
    }
    code = restore_page_device(i_ctx_p, igs, gs_gstate_saved(igs));
    if (code < 0) return code;
    if (code > 0)
        return push_callout(i_ctx_p, "%restorepagedevice");

    code = dorestore(i_ctx_p, asave);

    if (code < 0) {
         
        gs_currentdevice_inline(igs)->LockSafetyParams = saveLockSafety;
    }
    return code;
}
