SWriteFormat(ClientPtr client, xvFormat * pFormat)
{
    swapl(&pFormat->visual);
    WriteToClient(client, sz_xvFormat, pFormat);

    return Success;
}
