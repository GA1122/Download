clump_splay_app(clump_t *root, gs_ref_memory_t *imem, splay_app_result_t (*fn)(clump_t *, void *), void *arg)
{
    clump_t *step_to;
    clump_t *cp = root;
    int from = SPLAY_FROM_ABOVE;
    splay_app_result_t res;

    SANITY_CHECK(cp);

    while (cp)
    {
        if (from == SPLAY_FROM_ABOVE)
        {
             
            step_to = cp->left;
            if (step_to)
            {
                from = SPLAY_FROM_ABOVE;
                cp = step_to;
            }
            else
            {
                 
                from = SPLAY_FROM_LEFT;
            }
        }
        if (from == SPLAY_FROM_LEFT)
        {
             
            step_to = cp->right;
            if (step_to)
            {
                from = SPLAY_FROM_ABOVE;
                cp = step_to;
            }
            else
            {
                 
                from = SPLAY_FROM_RIGHT;
            }
        }
        if (from == SPLAY_FROM_RIGHT)
        {
             
            step_to = cp->parent;
            if (step_to)
            {
                from = (step_to->left == cp ? SPLAY_FROM_LEFT : SPLAY_FROM_RIGHT);
            }
            res = fn(cp, arg);
            if (res & SPLAY_APP_STOP)
                return cp;
            cp = step_to;
        }
    }
    return cp;
}
