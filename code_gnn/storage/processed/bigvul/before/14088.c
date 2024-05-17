RenderExtensionInit (void)
{
    ExtensionEntry *extEntry;

    if (!PictureType)
	return;
    if (!PictureFinishInit ())
	return;
    if (!dixRegisterPrivateKey(&RenderClientPrivateKeyRec, PRIVATE_CLIENT, sizeof(RenderClientRec)))
	return;
    if (!AddCallback (&ClientStateCallback, RenderClientCallback, 0))
	return;

    extEntry = AddExtension (RENDER_NAME, 0, RenderNumberErrors,
			     ProcRenderDispatch, SProcRenderDispatch,
			     NULL, StandardMinorOpcode);
    if (!extEntry)
	return;
    RenderErrBase = extEntry->errorBase;
#ifdef PANORAMIX
    if (XRT_PICTURE)
	SetResourceTypeErrorValue(XRT_PICTURE, RenderErrBase + BadPicture);
#endif
    SetResourceTypeErrorValue(PictureType, RenderErrBase + BadPicture);
    SetResourceTypeErrorValue(PictFormatType, RenderErrBase + BadPictFormat);
    SetResourceTypeErrorValue(GlyphSetType, RenderErrBase + BadGlyphSet);
}
