void GraphicsContext::fillPath(const Path& path)
{
#if USE(WXGC)
    wxGraphicsContext* gc = m_data->context->GetGraphicsContext();
    if (gc)
        gc->FillPath(*path.platformPath());
#endif
}
