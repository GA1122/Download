void WebView::setInputMethodState(bool enabled)
{
    Ime::ImmAssociateContextEx(m_window, 0, enabled ? IACE_DEFAULT : 0);
}
