void ShadowRoot::setApplyAuthorStyles(bool value)
{
    if (m_applyAuthorStyles != value) {
        m_applyAuthorStyles = value;
        if (attached() && owner())
            owner()->setNeedsRedistributing();
    }
}
