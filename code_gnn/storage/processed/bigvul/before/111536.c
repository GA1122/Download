TouchEventHandler::TouchEventHandler(WebPagePrivate* webpage)
    : m_webPage(webpage)
    , m_existingTouchMode(ProcessedTouchEvents)
    , m_shouldRequestSpellCheckOptions(false)
{
}
