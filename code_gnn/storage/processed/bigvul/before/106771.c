void WebView::unapplyEditCommand(WebEditCommandProxy* command)
{
    if (!m_page->isValid() || !m_page->isValidEditCommand(command))
        return;
    
    command->unapply();
}
