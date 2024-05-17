SwapXResQueryResourceBytes(struct xorg_list *response)
{
    struct xorg_list *it = response->next;
    int c;

    while (it != response) {
        xXResResourceSizeValue *value = FRAGMENT_DATA(it);
        it = it->next;
        for (c = 0; c < value->numCrossReferences; ++c) {
            xXResResourceSizeSpec *spec = FRAGMENT_DATA(it);
            SwapXResResourceSizeSpec(spec);
            it = it->next;
        }
        SwapXResResourceSizeValue(value);
    }
}
