clump_splay_remove(clump_t *cp, gs_ref_memory_t *imem)
{
    clump_t *replacement;

    if (cp->left == NULL)
    {
         
        replacement = cp->right;
    }
    else if (cp->right == NULL)
    {
         
        replacement = cp->left;
    }
    else
    {
         
         
        replacement = cp->left;
        while (replacement->right)
            replacement = replacement->right;
         
        (void)clump_splay_remove(replacement, imem);
         
        if (cp->left)
            cp->left->parent = replacement;
        cp->right->parent = replacement;
        replacement->left = cp->left;
        replacement->right = cp->right;
    }
    if (cp->parent)
    {
        if (cp->parent->left == cp)
            cp->parent->left = replacement;
        else
            cp->parent->right = replacement;
    }
    else
        imem->root = replacement;
    if (replacement)
        replacement->parent = cp->parent;
    return replacement;
}
