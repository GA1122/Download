PseudoramiXAddScreen(int x, int y, int w, int h)
{
    PseudoramiXScreenRec *s;

    if (noPseudoramiXExtension) return;

    if (pseudoramiXNumScreens == pseudoramiXScreensAllocated) {
        pseudoramiXScreensAllocated += pseudoramiXScreensAllocated + 1;
        pseudoramiXScreens = reallocarray(pseudoramiXScreens,
                                          pseudoramiXScreensAllocated,
                                          sizeof(PseudoramiXScreenRec));
    }

    DEBUG_LOG("x: %d, y: %d, w: %d, h: %d\n", x, y, w, h);

    s = &pseudoramiXScreens[pseudoramiXNumScreens++];
    s->x = x;
    s->y = y;
    s->w = w;
    s->h = h;
}
