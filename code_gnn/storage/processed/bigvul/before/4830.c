FilterRawEvents(const ClientPtr client, const GrabPtr grab, WindowPtr root)
{
    XIClientPtr client_xi_version;
    int cmp;

     
    if (!grab)
        return FALSE;

    client_xi_version =
        dixLookupPrivate(&client->devPrivates, XIClientPrivateKey);

    cmp = version_compare(client_xi_version->major_version,
                          client_xi_version->minor_version, 2, 0);
     
    if (cmp == 0)
        return TRUE;

    return (grab->window != root) ? FALSE : SameClient(grab, client);
}
