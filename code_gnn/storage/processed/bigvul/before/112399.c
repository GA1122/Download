void Document::captionPreferencesChanged()
{
    HashSet<Element*>::iterator end = m_captionPreferencesChangedElements.end();
    for (HashSet<Element*>::iterator it = m_captionPreferencesChangedElements.begin(); it != end; ++it)
        (*it)->captionPreferencesChanged();
}
