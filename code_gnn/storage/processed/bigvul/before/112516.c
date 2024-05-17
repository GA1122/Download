void Document::privateBrowsingStateDidChange() 
{
    HashSet<Element*>::iterator end = m_privateBrowsingStateChangedElements.end();
    for (HashSet<Element*>::iterator it = m_privateBrowsingStateChangedElements.begin(); it != end; ++it)
        (*it)->privateBrowsingStateDidChange();
}
