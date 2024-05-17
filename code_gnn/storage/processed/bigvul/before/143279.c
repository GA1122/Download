bool Document::shouldMergeWithLegacyDescription(ViewportDescription::Type origin) const
{
    return settings() && settings()->viewportMetaMergeContentQuirk() && m_legacyViewportDescription.isMetaViewportType() && m_legacyViewportDescription.type == origin;
}
