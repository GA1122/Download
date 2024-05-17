void Document::scheduleUseShadowTreeUpdate(SVGUseElement& element)
{
    m_useElementsNeedingUpdate.add(&element);
    scheduleLayoutTreeUpdateIfNeeded();
}
