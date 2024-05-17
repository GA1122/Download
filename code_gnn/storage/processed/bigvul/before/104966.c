AffineTransform GraphicsContext::getCTM() const
{ 
#if USE(WXGC)
    wxGraphicsContext* gc = m_data->context->GetGraphicsContext();
    if (gc) {
        wxGraphicsMatrix matrix = gc->GetTransform();
        double a, b, c, d, e, f;
        matrix.Get(&a, &b, &c, &d, &e, &f);
        return AffineTransform(a, b, c, d, e, f);
    }
#endif
    return AffineTransform();
}
