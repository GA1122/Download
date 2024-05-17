dmxPictureInit(ScreenPtr pScreen, PictFormatPtr formats, int nformats)
{
    DMXScreenInfo *dmxScreen = &dmxScreens[pScreen->myNum];
    PictureScreenPtr ps;

    if (!miPictureInit(pScreen, formats, nformats))
        return FALSE;

    if (!dixRegisterPrivateKey
        (&dmxPictPrivateKeyRec, PRIVATE_PICTURE, sizeof(dmxPictPrivRec)))
        return FALSE;

    ps = GetPictureScreen(pScreen);

    DMX_WRAP(CreatePicture, dmxCreatePicture, dmxScreen, ps);
    DMX_WRAP(DestroyPicture, dmxDestroyPicture, dmxScreen, ps);

    DMX_WRAP(ChangePictureClip, dmxChangePictureClip, dmxScreen, ps);
    DMX_WRAP(DestroyPictureClip, dmxDestroyPictureClip, dmxScreen, ps);

    DMX_WRAP(ChangePicture, dmxChangePicture, dmxScreen, ps);
    DMX_WRAP(ValidatePicture, dmxValidatePicture, dmxScreen, ps);

    DMX_WRAP(Composite, dmxComposite, dmxScreen, ps);
    DMX_WRAP(Glyphs, dmxGlyphs, dmxScreen, ps);
    DMX_WRAP(CompositeRects, dmxCompositeRects, dmxScreen, ps);

    DMX_WRAP(Trapezoids, dmxTrapezoids, dmxScreen, ps);
    DMX_WRAP(Triangles, dmxTriangles, dmxScreen, ps);

    return TRUE;
}
