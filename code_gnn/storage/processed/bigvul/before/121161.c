HTMLImageLoader* HTMLInputElement::imageLoader()
{
    if (!m_imageLoader)
        m_imageLoader = adoptPtr(new HTMLImageLoader(this));
    return m_imageLoader.get();
}
