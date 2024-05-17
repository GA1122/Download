PassRefPtr<RenderStyle> HTMLInputElement::customStyleForRenderer()
{
    return m_inputType->customStyleForRenderer(originalStyleForRenderer());
}
