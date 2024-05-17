GraphicsContextPlatformPrivate::GraphicsContextPlatformPrivate() :
    context(0),
    mswDCStateID(0),
    gtkCurrentClipRgn(wxRegion()),
    gtkPaintClipRgn(wxRegion())
{
}
