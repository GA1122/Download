clump_splay_walk_fwd(clump_splay_walker *sw)
{
    clump_t *cp = sw->cp;
    int from = sw->from;

    if (cp == NULL)
        return NULL;

     
    while (1)
    {
        if (from == SPLAY_FROM_ABOVE)
        {
             
            if (cp->left)
            {
                cp = cp->left;
                from = SPLAY_FROM_ABOVE;
                continue;
            }
             
            from = SPLAY_FROM_LEFT;
             
            if (cp == sw->end)
                cp = NULL;
             
            break;
        }
        if (from == SPLAY_FROM_LEFT)
        {
             
            if (cp->right)
            {
                cp = cp->right;
                from = SPLAY_FROM_ABOVE;
                continue;
            }
             
            from = SPLAY_FROM_RIGHT;
        }
        if (from == SPLAY_FROM_RIGHT)
        {
             
            clump_t *old = cp;
            cp = cp->parent;
            if (cp == NULL)
            {
                 
                if (sw->end == NULL)
                    break;
                 
                cp = old;
                from = SPLAY_FROM_ABOVE;
            }
            else
            {
                from = (cp->left == old ? SPLAY_FROM_LEFT : SPLAY_FROM_RIGHT);
                if (from == SPLAY_FROM_LEFT)
                {
                     
                    if (cp == sw->end)
                        cp = NULL;
                    break;
                }
            }
        }
    }
    sw->cp = cp;
    sw->from = from;
    return cp;
}
