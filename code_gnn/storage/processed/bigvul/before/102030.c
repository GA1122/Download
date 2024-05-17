PassRefPtr<GraphicsSurface> GraphicsSurface::platformImport(const IntSize& size, Flags flags, const GraphicsSurfaceToken& token)
{
    if (flags & SupportsCopyToTexture || flags & SupportsSingleBuffered)
        return PassRefPtr<GraphicsSurface>();

    RefPtr<GraphicsSurface> surface = adoptRef(new GraphicsSurface(size, flags));

    surface->m_private = new GraphicsSurfacePrivate();
    if (!resolveGLMethods(surface->m_private))
        return PassRefPtr<GraphicsSurface>();

    surface->m_platformSurface = token.frontBufferHandle;

    surface->m_private->createPixmap(surface->m_platformSurface);
    surface->m_size = surface->m_private->size();

    return surface;
}
