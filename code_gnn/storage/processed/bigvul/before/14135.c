DoMakeCurrent(__GLXclientState *cl,
	      GLXDrawable drawId, GLXDrawable readId,
	      GLXContextID contextId, GLXContextTag tag)
{
    ClientPtr client = cl->client;
    xGLXMakeCurrentReply reply;
    __GLXcontext *glxc, *prevglxc;
    __GLXdrawable *drawPriv = NULL;
    __GLXdrawable *readPriv = NULL;
    int error;
    GLuint  mask;

     

    mask  = (drawId == None)    ? (1 << 0) : 0;
    mask |= (readId == None)    ? (1 << 1) : 0;
    mask |= (contextId == None) ? (1 << 2) : 0;

    if ( (mask != 0x00) && (mask != 0x07) ) {
	return BadMatch;
    }
    
     
    if (tag != 0) {
	prevglxc = __glXLookupContextByTag(cl, tag);
	if (!prevglxc) {
	     
	    return __glXError(GLXBadContextTag);
	}
	if (prevglxc->renderMode != GL_RENDER) {
	     
	    client->errorValue = prevglxc->id;
	    return __glXError(GLXBadContextState);
	}
    } else {
	prevglxc = 0;
    }

     
    if (contextId != None) {
	int  status;

	if (!validGlxContext(client, contextId, DixUseAccess, &glxc, &error))
	    return error;
	if ((glxc != prevglxc) && glxc->isCurrent) {
	     
	    return BadAccess;
	}

	assert( drawId != None );
	assert( readId != None );

	drawPriv = __glXGetDrawable(glxc, drawId, client, &status);
	if (drawPriv == NULL)
	    return status;

	readPriv = __glXGetDrawable(glxc, readId, client, &status);
	if (readPriv == NULL)
	    return status;

    } else {
	 
	glxc = 0;
	drawPriv = 0;
	readPriv = 0;
    }


    if (prevglxc) {
	 
	if (__GLX_HAS_UNFLUSHED_CMDS(prevglxc)) {
	    if (__glXForceCurrent(cl, tag, (int *)&error)) {
		CALL_Flush( GET_DISPATCH(), () );
		__GLX_NOTE_FLUSHED_CMDS(prevglxc);
	    } else {
		return error;
	    }
	}

	 
	if (!(*prevglxc->loseCurrent)(prevglxc)) {
	    return __glXError(GLXBadContext);
	}
	__glXFlushContextCache();
	if (!prevglxc->isDirect) {
	    prevglxc->drawPriv = NULL;
	    prevglxc->readPriv = NULL;
	}
    }
	

    if ((glxc != 0) && !glxc->isDirect) {

	glxc->drawPriv = drawPriv;
	glxc->readPriv = readPriv;

	 
	if (!(*glxc->makeCurrent)(glxc)) {
	    glxc->drawPriv = NULL;
	    glxc->readPriv = NULL;
	    return __glXError(GLXBadContext);
	}

	glxc->isCurrent = GL_TRUE;
    }

    if (prevglxc) {
	ChangeCurrentContext(cl, glxc, tag);
	StopUsingContext(prevglxc);
    } else {
	tag = AddCurrentContext(cl, glxc);
    }

    if (glxc) {
	StartUsingContext(cl, glxc);
	reply.contextTag = tag;
    } else {
	reply.contextTag = 0;
    }

    reply.length = 0;
    reply.type = X_Reply;
    reply.sequenceNumber = client->sequence;

    if (client->swapped) {
	__glXSwapMakeCurrentReply(client, &reply);
    } else {
	WriteToClient(client, sz_xGLXMakeCurrentReply, (char *)&reply);
    }
    return Success;
}
