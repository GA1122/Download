LRESULT WebView::onIMERequestReconvertString(RECONVERTSTRING* reconvertString)
{
    String text = m_page->getSelectedText();
    unsigned totalSize = sizeof(RECONVERTSTRING) + text.length() * sizeof(UChar);
    
    if (!reconvertString)
        return totalSize;

    if (totalSize > reconvertString->dwSize)
        return 0;
    reconvertString->dwCompStrLen = text.length();
    reconvertString->dwStrLen = text.length();
    reconvertString->dwTargetStrLen = text.length();
    reconvertString->dwStrOffset = sizeof(RECONVERTSTRING);
    memcpy(reconvertString + 1, text.characters(), text.length() * sizeof(UChar));
    return totalSize;
}
