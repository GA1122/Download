 XRRGetProviderResources(Display *dpy, Window window)
 {
     XExtDisplayInfo		*info = XRRFindDisplay(dpy);
     xRRGetProvidersReply rep;
     xRRGetProvidersReq *req;
     XRRProviderResources *xrpr;
     long nbytes, nbytesRead;
     int rbytes;
 
     RRCheckExtension (dpy, info, NULL);
 
     LockDisplay (dpy);
 
     GetReq(RRGetProviders, req);
     req->reqType = info->codes->major_opcode;
     req->randrReqType = X_RRGetProviders;
     req->window = window;
     
     if (!_XReply (dpy, (xReply *) &rep, 0, xFalse))
     {
       UnlockDisplay (dpy);
       SyncHandle ();
       return NULL;
        return NULL;
      }
  
    nbytes = (long) rep.length << 2;
//     if (rep.length < INT_MAX >> 2) {
// 	nbytes = (long) rep.length << 2;
  
    nbytesRead = (long) (rep.nProviders * 4);
// 	nbytesRead = (long) (rep.nProviders * 4);
  
    rbytes = (sizeof(XRRProviderResources) + rep.nProviders * sizeof(RRProvider));
    xrpr = (XRRProviderResources *) Xmalloc(rbytes);
// 	rbytes = (sizeof(XRRProviderResources) + rep.nProviders *
// 		  sizeof(RRProvider));
// 	xrpr = (XRRProviderResources *) Xmalloc(rbytes);
//     } else {
// 	nbytes = 0;
// 	nbytesRead = 0;
// 	rbytes = 0;
// 	xrpr = NULL;
//     }
  
      if (xrpr == NULL) {
         _XEatDataWords (dpy, rep.length);
     _XRead32(dpy, (long *) xrpr->providers, rep.nProviders << 2);
 
     if (nbytes > nbytesRead)
       _XEatData (dpy, (unsigned long) (nbytes - nbytesRead));
 
 
     UnlockDisplay (dpy);
     SyncHandle();
 
     return (XRRProviderResources *) xrpr;
 }
 
 void
 XRRFreeProviderResources(XRRProviderResources *provider_resources)
 {
     free(provider_resources);
 }
 
 #define ProviderInfoExtra	(SIZEOF(xRRGetProviderInfoReply) - 32)  
 XRRProviderInfo *
 XRRGetProviderInfo(Display *dpy, XRRScreenResources *resources, RRProvider provider)
 {
     XExtDisplayInfo	    *info = XRRFindDisplay(dpy);
     xRRGetProviderInfoReply rep;
     xRRGetProviderInfoReq *req;
     int nbytes, nbytesRead, rbytes;
     XRRProviderInfo *xpi;
 
     RRCheckExtension (dpy, info, NULL);
 
     LockDisplay (dpy);
     GetReq (RRGetProviderInfo, req);
     req->reqType = info->codes->major_opcode;
     req->randrReqType = X_RRGetProviderInfo;
     req->provider = provider;
     req->configTimestamp = resources->configTimestamp;
 
     if (!_XReply (dpy, (xReply *) &rep, ProviderInfoExtra >> 2, xFalse))
     {
 	UnlockDisplay (dpy);
 	SyncHandle ();
 	return NULL;
     }
 
     nbytes = ((long) rep.length << 2) - ProviderInfoExtra;
 
     nbytesRead = (long)(rep.nCrtcs * 4 +
 			rep.nOutputs * 4 +
 			rep.nAssociatedProviders * 8 +
 			((rep.nameLength + 3) & ~3));
  	return NULL;
      }
  
//     if (rep.length > INT_MAX >> 2 || rep.length < ProviderInfoExtra >> 2)
//     {
// 	if (rep.length < ProviderInfoExtra >> 2)
// 	    _XEatDataWords (dpy, rep.length);
// 	else
// 	    _XEatDataWords (dpy, rep.length - (ProviderInfoExtra >> 2));
// 	UnlockDisplay (dpy);
// 	SyncHandle ();
// 	return NULL;
//     }
// 
      nbytes = ((long) rep.length << 2) - ProviderInfoExtra;
  
      nbytesRead = (long)(rep.nCrtcs * 4 +
     xpi->noutputs = rep.nOutputs;
     xpi->nassociatedproviders = rep.nAssociatedProviders;
     xpi->crtcs = (RRCrtc *)(xpi + 1);
     xpi->outputs = (RROutput *)(xpi->crtcs + rep.nCrtcs);
     xpi->associated_providers = (RRProvider *)(xpi->outputs + rep.nOutputs);
     xpi->associated_capability = (unsigned int *)(xpi->associated_providers + rep.nAssociatedProviders);
     xpi->name = (char *)(xpi->associated_capability + rep.nAssociatedProviders);
 
     _XRead32(dpy, (long *) xpi->crtcs, rep.nCrtcs << 2);
     _XRead32(dpy, (long *) xpi->outputs, rep.nOutputs << 2);
 
     _XRead32(dpy, (long *) xpi->associated_providers, rep.nAssociatedProviders << 2);
 
      
     _XRead(dpy, (char *) xpi->associated_capability,
            rep.nAssociatedProviders << 2);
 
     _XReadPad(dpy, xpi->name, rep.nameLength);
     xpi->name[rep.nameLength] = '\0';
 
      
     if (nbytes > nbytesRead)
 	_XEatData (dpy, (unsigned long) (nbytes - nbytesRead));
 
     UnlockDisplay (dpy);
     SyncHandle ();
     return (XRRProviderInfo *) xpi;
 }