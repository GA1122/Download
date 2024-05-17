void GraphicsContext::rotate(float angle) 
{ 
#if USE(WXGC)
    if (m_data->context) {
        wxGraphicsContext* gc = m_data->context->GetGraphicsContext();
        gc->Rotate(angle);
    }
#endif
}
