PanoramiXConsolidate(void)
{
    int i;
    PanoramiXRes *root, *defmap, *saver;
    ScreenPtr pScreen = screenInfo.screens[0];
    DepthPtr pDepth = pScreen->allowedDepths;
    VisualPtr pVisual = pScreen->visuals;

    PanoramiXNumDepths = 0;
    PanoramiXNumVisuals = 0;

    for (i = 0; i < pScreen->numDepths; i++)
        PanoramiXMaybeAddDepth(pDepth++);

    for (i = 0; i < pScreen->numVisuals; i++)
        PanoramiXMaybeAddVisual(pVisual++);

    root = malloc(sizeof(PanoramiXRes));
    root->type = XRT_WINDOW;
    defmap = malloc(sizeof(PanoramiXRes));
    defmap->type = XRT_COLORMAP;
    saver = malloc(sizeof(PanoramiXRes));
    saver->type = XRT_WINDOW;

    FOR_NSCREENS(i) {
        ScreenPtr scr = screenInfo.screens[i];

        root->info[i].id = scr->root->drawable.id;
        root->u.win.class = InputOutput;
        root->u.win.root = TRUE;
        saver->info[i].id = scr->screensaver.wid;
        saver->u.win.class = InputOutput;
        saver->u.win.root = TRUE;
        defmap->info[i].id = scr->defColormap;
    }

    AddResource(root->info[0].id, XRT_WINDOW, root);
    AddResource(saver->info[0].id, XRT_WINDOW, saver);
    AddResource(defmap->info[0].id, XRT_COLORMAP, defmap);
}
