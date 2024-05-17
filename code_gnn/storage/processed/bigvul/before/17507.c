SWriteImageFormatInfo(ClientPtr client, xvImageFormatInfo * pImage)
{
    swapl(&pImage->id);
    swapl(&pImage->red_mask);
    swapl(&pImage->green_mask);
    swapl(&pImage->blue_mask);
    swapl(&pImage->y_sample_bits);
    swapl(&pImage->u_sample_bits);
    swapl(&pImage->v_sample_bits);
    swapl(&pImage->horz_y_period);
    swapl(&pImage->horz_u_period);
    swapl(&pImage->horz_v_period);
    swapl(&pImage->vert_y_period);
    swapl(&pImage->vert_u_period);
    swapl(&pImage->vert_v_period);

    WriteToClient(client, sz_xvImageFormatInfo, pImage);

    return Success;
}
