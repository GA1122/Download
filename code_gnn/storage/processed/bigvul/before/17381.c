VisualsEqual(VisualPtr a, ScreenPtr pScreenB, VisualPtr b)
{
    return ((a->class == b->class) &&
            (a->ColormapEntries == b->ColormapEntries) &&
            (a->nplanes == b->nplanes) &&
            (a->redMask == b->redMask) &&
            (a->greenMask == b->greenMask) &&
            (a->blueMask == b->blueMask) &&
            (a->offsetRed == b->offsetRed) &&
            (a->offsetGreen == b->offsetGreen) &&
            (a->offsetBlue == b->offsetBlue));
}
