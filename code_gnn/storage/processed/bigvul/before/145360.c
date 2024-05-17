bool FrameLoader::shouldClose(bool isReload)
{
    Page* page = m_frame->page();
    if (!page || !page->chromeClient().canOpenBeforeUnloadConfirmPanel())
        return true;

    HeapVector<Member<LocalFrame>> targetFrames;
    targetFrames.append(m_frame);
    for (Frame* child = m_frame->tree().firstChild(); child; child = child->tree().traverseNext(m_frame)) {
        if (child->isLocalFrame())
            targetFrames.append(toLocalFrame(child));
    }

    bool shouldClose = false;
    {
        NavigationDisablerForBeforeUnload navigationDisabler;
        size_t i;

        bool didAllowNavigation = false;
        for (i = 0; i < targetFrames.size(); i++) {
            if (!targetFrames[i]->tree().isDescendantOf(m_frame))
                continue;
            if (!targetFrames[i]->document()->dispatchBeforeUnloadEvent(page->chromeClient(), isReload, didAllowNavigation))
                break;
        }

        if (i == targetFrames.size())
            shouldClose = true;
    }
    return shouldClose;
}
