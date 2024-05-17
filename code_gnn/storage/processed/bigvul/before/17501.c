SWriteAdaptorInfo(ClientPtr client, xvAdaptorInfo * pAdaptor)
{
    swapl(&pAdaptor->base_id);
    swaps(&pAdaptor->name_size);
    swaps(&pAdaptor->num_ports);
    swaps(&pAdaptor->num_formats);

    WriteToClient(client, sz_xvAdaptorInfo, pAdaptor);

    return Success;
}
