XineramifyXv(void)
{
    XvScreenPtr xvsp0 =
        dixLookupPrivate(&screenInfo.screens[0]->devPrivates, XvGetScreenKey());
    XvAdaptorPtr MatchingAdaptors[MAXSCREENS];
    int i, j, k;

    XvXRTPort = CreateNewResourceType(XineramaDeleteResource, "XvXRTPort");

    if (!xvsp0 || !XvXRTPort)
        return;
    SetResourceTypeErrorValue(XvXRTPort, _XvBadPort);

    for (i = 0; i < xvsp0->nAdaptors; i++) {
        Bool isOverlay;
        XvAdaptorPtr refAdapt = xvsp0->pAdaptors + i;

        if (!(refAdapt->type & XvInputMask))
            continue;

        MatchingAdaptors[0] = refAdapt;
        isOverlay = hasOverlay(refAdapt);
        FOR_NSCREENS_FORWARD_SKIP(j)
            MatchingAdaptors[j] =
            matchAdaptor(screenInfo.screens[j], refAdapt, isOverlay);

         
        for (j = 0; j < refAdapt->nPorts; j++) {
            PanoramiXRes *port = malloc(sizeof(PanoramiXRes));

            if (!port)
                break;

            FOR_NSCREENS(k) {
                if (MatchingAdaptors[k] && (MatchingAdaptors[k]->nPorts > j))
                    port->info[k].id = MatchingAdaptors[k]->base_id + j;
                else
                    port->info[k].id = 0;
            }
            AddResource(port->info[0].id, XvXRTPort, port);
        }
    }

     
    XvProcVector[xv_PutVideo] = XineramaXvPutVideo;
    XvProcVector[xv_PutStill] = XineramaXvPutStill;
    XvProcVector[xv_StopVideo] = XineramaXvStopVideo;
    XvProcVector[xv_SetPortAttribute] = XineramaXvSetPortAttribute;
    XvProcVector[xv_PutImage] = XineramaXvPutImage;
    XvProcVector[xv_ShmPutImage] = XineramaXvShmPutImage;
}
