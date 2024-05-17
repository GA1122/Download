SWriteEncodingInfo(ClientPtr client, xvEncodingInfo * pEncoding)
{

    swapl(&pEncoding->encoding);
    swaps(&pEncoding->name_size);
    swaps(&pEncoding->width);
    swaps(&pEncoding->height);
    swapl(&pEncoding->rate.numerator);
    swapl(&pEncoding->rate.denominator);
    WriteToClient(client, sz_xvEncodingInfo, pEncoding);

    return Success;
}
