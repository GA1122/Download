bool WebPage::deleteTextRelativeToCursor(unsigned int leftOffset, unsigned int rightOffset)
{
    if (d->m_page->defersLoading())
        return false;

    return d->m_inputHandler->deleteTextRelativeToCursor(leftOffset, rightOffset);
}
