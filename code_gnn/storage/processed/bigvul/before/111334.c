int32_t WebPage::setComposingText(spannable_string_t* spannableString, int32_t relativeCursorPosition)
{
    if (d->m_page->defersLoading())
        return -1;
    return d->m_inputHandler->setComposingText(spannableString, relativeCursorPosition);
}
