void ChromeClientImpl::didAssociateFormControls(const Vector<RefPtr<Element> >& elements)
{
    if (!m_webView->autofillClient())
        return;
    WebVector<WebNode> elementVector(static_cast<size_t>(elements.size()));
    size_t elementsCount = elements.size();
    for (size_t i = 0; i < elementsCount; ++i)
        elementVector[i] = elements[i];
    m_webView->autofillClient()->didAssociateFormControls(elementVector);
}
