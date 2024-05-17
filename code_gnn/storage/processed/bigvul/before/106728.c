LRESULT WebView::onIMERequest(WPARAM request, LPARAM data)
{
    LOG(TextInput, "onIMERequest %s", imeRequestName(request).latin1().data());
    if (!m_page->selectionState().isContentEditable)
        return 0;

    switch (request) {
    case IMR_RECONVERTSTRING:
        return onIMERequestReconvertString(reinterpret_cast<RECONVERTSTRING*>(data));

    case IMR_QUERYCHARPOSITION:
        return onIMERequestCharPosition(reinterpret_cast<IMECHARPOSITION*>(data));
    }
    return 0;
}
