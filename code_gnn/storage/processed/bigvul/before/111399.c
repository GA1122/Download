BlackBerry::Platform::String WebPage::title() const
{
    if (d->m_mainFrame->document())
        return d->m_mainFrame->loader()->documentLoader()->title().string();
    return BlackBerry::Platform::String::emptyString();
}
