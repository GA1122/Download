void Document::windowScreenDidChange(PlatformDisplayID displayID)
{
    UNUSED_PARAM(displayID);

#if ENABLE(REQUEST_ANIMATION_FRAME)
    if (m_scriptedAnimationController)
        m_scriptedAnimationController->windowScreenDidChange(displayID);
#endif

#if USE(ACCELERATED_COMPOSITING)
    if (RenderView* view = renderView()) {
        if (view->usesCompositing())
            view->compositor()->windowScreenDidChange(displayID);
    }
#endif
}
