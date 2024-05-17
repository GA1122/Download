void WebPage::spellCheckingRequestProcessed(int32_t transactionId, spannable_string_t* spannableString)
{
    d->m_inputHandler->spellCheckingRequestProcessed(transactionId, spannableString);
}
