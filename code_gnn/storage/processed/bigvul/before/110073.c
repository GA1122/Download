bool HTMLSelectElement::usesMenuList() const
{
    const Page* page = document()->page();
    RefPtr<RenderTheme> renderTheme = page ? page->theme() : RenderTheme::defaultTheme();
    if (renderTheme->delegatesMenuListRendering())
        return true;

    return !m_multiple && m_size <= 1;
}
