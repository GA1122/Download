void WebPageProxy::showCorrectionPanel(int32_t panelType, const WebCore::FloatRect& boundingBoxOfReplacedString, const String& replacedString, const String& replacementString, const Vector<String>& alternativeReplacementStrings)
{
    m_pageClient->showCorrectionPanel((WebCore::CorrectionPanelInfo::PanelType)panelType, boundingBoxOfReplacedString, replacedString, replacementString, alternativeReplacementStrings);
}
