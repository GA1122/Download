WriteFragmentsToClient(ClientPtr client, struct xorg_list *frags)
{
    FragmentList *it;
    xorg_list_for_each_entry(it, frags, l) {
        WriteToClient(client, it->bytes, (char*) it + sizeof(*it));
    }
}
