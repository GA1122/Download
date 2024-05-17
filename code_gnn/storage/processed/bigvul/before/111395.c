spannable_string_t* WebPage::textAfterCursor(int32_t length, int32_t flags)
{
    return d->m_inputHandler->textAfterCursor(length, flags);
}
