DoCreatePbuffer(ClientPtr client, int screenNum, XID fbconfigId,
		int width, int height, XID glxDrawableId)
{
    __GLXconfig	*config;
    __GLXscreen		*pGlxScreen;
    PixmapPtr		 pPixmap;
    int			 err;

    LEGAL_NEW_RESOURCE(glxDrawableId, client);

    if (!validGlxScreen(client, screenNum, &pGlxScreen, &err))
	return err;
    if (!validGlxFBConfig(client, pGlxScreen, fbconfigId, &config, &err))
	return err;

    __glXenterServer(GL_FALSE);
    pPixmap = (*pGlxScreen->pScreen->CreatePixmap) (pGlxScreen->pScreen,
						    width, height, config->rgbBits, 0);
    __glXleaveServer(GL_FALSE);

     
    pPixmap->drawable.id = glxDrawableId;
    if (!AddResource(pPixmap->drawable.id, RT_PIXMAP, pPixmap))
	return BadAlloc;

    return DoCreateGLXDrawable(client, pGlxScreen, config, &pPixmap->drawable,
			       glxDrawableId, glxDrawableId,
			       GLX_DRAWABLE_PBUFFER);
}