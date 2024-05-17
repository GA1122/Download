void Document::webkitCancelFullScreen()
{
    if (!topDocument()->webkitFullscreenElement())
        return;

    Vector<RefPtr<Element> > replacementFullscreenElementStack;
    replacementFullscreenElementStack.append(topDocument()->webkitFullscreenElement());
    topDocument()->m_fullScreenElementStack.swap(replacementFullscreenElementStack);

    topDocument()->webkitExitFullscreen();
}
