void WebView::setCursor(const WebCore::Cursor& cursor)
{
    if (!cursor.platformCursor()->nativeCursor())
        return;
    m_webCoreCursor = cursor.platformCursor()->nativeCursor();
    updateNativeCursor();
}
