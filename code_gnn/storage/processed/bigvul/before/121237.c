void HTMLInputElement::selectColorInColorChooser(const Color& color)
{
    if (!m_inputType->isColorControl())
        return;
    static_cast<ColorInputType*>(m_inputType.get())->didChooseColor(color);
}
