static int AddCurrentContext(__GLXclientState *cl, __GLXcontext *glxc)
{
    int i;
    int num = cl->numCurrentContexts;
    __GLXcontext **table = cl->currentContexts;

    if (!glxc) return -1;
    
     
    for (i=0; i < num; i++) {
	if (!table[i]) {
	    table[i] = glxc;
	    return i+1;
	}
    }
     
    if (!num) {
	table = (__GLXcontext **) malloc(sizeof(__GLXcontext *));
    } else {
	table = (__GLXcontext **) realloc(table,
					   (num+1)*sizeof(__GLXcontext *));
    }
    table[num] = glxc;
    cl->currentContexts = table;
    cl->numCurrentContexts++;
    return num+1;
}
