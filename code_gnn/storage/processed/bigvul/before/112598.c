void Document::unregisterForCaptionPreferencesChangedCallbacks(Element* e)
{
    m_captionPreferencesChangedElements.remove(e);
}
