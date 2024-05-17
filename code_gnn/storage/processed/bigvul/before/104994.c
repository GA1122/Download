void GraphicsContext::translate(float tx, float ty) 
{ 
#if USE(WXGC)
    if (m_data->context) {
        wxGraphicsContext* gc = m_data->context->GetGraphicsContext();
        gc->Translate(tx, ty);
    }
#endif
}
