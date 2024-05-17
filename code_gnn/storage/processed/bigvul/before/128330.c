bool FrameView::scrollToFragment(const KURL& url)
{
    if (!url.hasFragmentIdentifier() && !m_frame->document()->cssTarget())
        return false;

    String fragmentIdentifier = url.fragmentIdentifier();
    if (scrollToAnchor(fragmentIdentifier))
        return true;

    if (m_frame->document()->encoding().isValid())
        return scrollToAnchor(decodeURLEscapeSequences(fragmentIdentifier, m_frame->document()->encoding()));

    return false;
}
