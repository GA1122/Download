int __glXDisp_CreateWindow(__GLXclientState *cl, GLbyte *pc)
{
    xGLXCreateWindowReq	*req = (xGLXCreateWindowReq *) pc;
    __GLXconfig	*config;
    __GLXscreen		*pGlxScreen;
    ClientPtr		 client = cl->client;
    DrawablePtr		 pDraw;
    int			 err;

    LEGAL_NEW_RESOURCE(req->glxwindow, client);

    if (!validGlxScreen(client, req->screen, &pGlxScreen, &err))
	return err;
    if (!validGlxFBConfig(client, pGlxScreen, req->fbconfig, &config, &err))
	return err;

    err = dixLookupDrawable(&pDraw, req->window, client, 0, DixAddAccess);
    if (err != Success || pDraw->type != DRAWABLE_WINDOW) {
	client->errorValue = req->window;
	return BadWindow;
    }

    if (!validGlxFBConfigForWindow(client, config, pDraw, &err))
	return err;

    return DoCreateGLXDrawable(client, pGlxScreen, config,
			       pDraw, req->window,
			       req->glxwindow, GLX_DRAWABLE_WINDOW);
}
