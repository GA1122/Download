extracted_text_t* WebPage::extractedTextRequest(extracted_text_request_t* request, int32_t flags)
{
    return d->m_inputHandler->extractedTextRequest(request, flags);
}
