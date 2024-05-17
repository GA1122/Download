void Document::unscheduleUseShadowTreeUpdate(SVGUseElement& element)
{
    m_useElementsNeedingUpdate.remove(&element);
}
