static void StopUsingContext(__GLXcontext *glxc)
{
    if (glxc) {
	if (glxc == __glXLastContext) {
	     
	    __glXLastContext = 0;
	}
	glxc->isCurrent = GL_FALSE;
	if (!glxc->idExists) {
	    __glXFreeContext(glxc);
	}
    }
}
