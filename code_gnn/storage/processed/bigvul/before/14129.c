DoCreateContext(__GLXclientState *cl, GLXContextID gcId,
		GLXContextID shareList, __GLXconfig *config,
		__GLXscreen *pGlxScreen, GLboolean isDirect)
{
    ClientPtr client = cl->client;
    __GLXcontext *glxc, *shareglxc;
    int err;
    
    LEGAL_NEW_RESOURCE(gcId, client);

     
    if (shareList == None) {
	shareglxc = 0;
    } else {
	if (!validGlxContext(client, shareList, DixReadAccess,
			     &shareglxc, &err))
	    return err;

	if (shareglxc->isDirect) {
	     
#if 0
             
	    client->errorValue = shareList;
	    return BadMatch;
#endif
	} else {
	     
	    isDirect = GL_FALSE;
	}
    }

     
    if (!isDirect)
	glxc = pGlxScreen->createContext(pGlxScreen, config, shareglxc);
    else
	glxc = __glXdirectContextCreate(pGlxScreen, config, shareglxc);
    if (!glxc) {
	return BadAlloc;
    }

     
    glxc->pGlxScreen = pGlxScreen;
    glxc->config = config;

     
    if (!AddResource(gcId, __glXContextRes, (pointer)glxc)) {
	(*glxc->destroy)(glxc);
	client->errorValue = gcId;
	return BadAlloc;
    }
    
     
    glxc->id = gcId;
    glxc->share_id = shareList;
    glxc->idExists = GL_TRUE;
    glxc->isCurrent = GL_FALSE;
    glxc->isDirect = isDirect;
    glxc->renderMode = GL_RENDER;

    __glXAddToContextList(glxc);

    return Success;
}
