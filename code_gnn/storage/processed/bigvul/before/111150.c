void WebPagePrivate::exitFullScreenForElement(Element* element)
{
#if ENABLE(VIDEO)
    if (!element || (!element->hasTagName(HTMLNames::videoTag) && !containsVideoTags(element)))
        return;
    if (m_webSettings->fullScreenVideoCapable()) {
        exitFullscreenForNode(element);
    } else {
        client()->fullscreenStop();
        m_fullscreenVideoNode = 0;
    }
#endif
}
