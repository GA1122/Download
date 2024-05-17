DestroyFragments(struct xorg_list *frags)
{
    FragmentList *it, *tmp;
    xorg_list_for_each_entry_safe(it, tmp, frags, l) {
        xorg_list_del(&it->l);
        free(it);
    }
}
