bool HTMLConstructionSite::shouldFosterParent() const
{
    return m_redirectAttachToFosterParent
        && causesFosterParenting(currentElement()->tagQName());
}
