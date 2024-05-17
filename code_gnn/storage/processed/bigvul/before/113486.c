void WebPagePrivate::exitFullScreenForElement(Element* element)
{
#if ENABLE(VIDEO)
    if (!element)
        return;
    if (m_webSettings->fullScreenVideoCapable() && element->hasTagName(HTMLNames::videoTag)) {
        exitFullscreenForNode(element);
    } else {
        client()->fullscreenStop();
        m_fullscreenNode = 0;
    }
#endif
}
