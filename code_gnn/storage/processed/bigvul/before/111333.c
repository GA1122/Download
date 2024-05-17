int32_t WebPage::setComposingRegion(int32_t start, int32_t end)
{
    return d->m_inputHandler->setComposingRegion(start, end);
}
