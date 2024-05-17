dmxFindFormat(DMXScreenInfo * dmxScreen, PictFormatPtr pFmt)
{
    XRenderPictFormat *pFormat = NULL;
    int i = 0;

    if (!pFmt || !dmxScreen->beDisplay)
        return pFormat;

    while (1) {
        pFormat = XRenderFindFormat(dmxScreen->beDisplay, 0, 0, i++);
        if (!pFormat)
            break;

        if (pFormat->type != pFmt->type)
            continue;
        if (pFormat->depth != pFmt->depth)
            continue;
        if (pFormat->direct.red != pFmt->direct.red)
            continue;
        if (pFormat->direct.redMask != pFmt->direct.redMask)
            continue;
        if (pFormat->direct.green != pFmt->direct.green)
            continue;
        if (pFormat->direct.greenMask != pFmt->direct.greenMask)
            continue;
        if (pFormat->direct.blue != pFmt->direct.blue)
            continue;
        if (pFormat->direct.blueMask != pFmt->direct.blueMask)
            continue;
        if (pFormat->direct.alpha != pFmt->direct.alpha)
            continue;
        if (pFormat->direct.alphaMask != pFmt->direct.alphaMask)
            continue;

         
        break;
    }

    return pFormat;
}
