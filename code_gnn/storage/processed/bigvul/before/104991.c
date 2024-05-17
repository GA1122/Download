void GraphicsContext::strokePath(const Path& path)
{
#if USE(WXGC)
    wxGraphicsContext* gc = m_data->context->GetGraphicsContext();
    if (gc)
        gc->StrokePath(*path.platformPath());
#endif
}
