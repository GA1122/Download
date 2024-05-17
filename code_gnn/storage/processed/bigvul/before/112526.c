void Document::registerForCaptionPreferencesChangedCallbacks(Element* e)
{
    if (page())
        page()->group().captionPreferences()->setInterestedInCaptionPreferenceChanges();

    m_captionPreferencesChangedElements.add(e);
}
