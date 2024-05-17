PassRefPtr<GraphicsSurface> GraphicsSurface::platformCreate(const IntSize& size, Flags flags, const PlatformGraphicsContext3D shareContext)
{
    if (flags & SupportsCopyToTexture || flags & SupportsSingleBuffered)
        return PassRefPtr<GraphicsSurface>();

    RefPtr<GraphicsSurface> surface = adoptRef(new GraphicsSurface(size, flags));

    surface->m_private = new GraphicsSurfacePrivate(shareContext);
    if (!resolveGLMethods(surface->m_private))
        return PassRefPtr<GraphicsSurface>();

    surface->m_platformSurface = surface->m_private->createSurface(size);

    return surface;
}
