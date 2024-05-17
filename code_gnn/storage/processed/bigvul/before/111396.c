spannable_string_t* WebPage::textBeforeCursor(int32_t length, int32_t flags)
{
    return d->m_inputHandler->textBeforeCursor(length, flags);
}
