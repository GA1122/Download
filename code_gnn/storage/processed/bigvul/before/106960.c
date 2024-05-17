void QQuickWebViewFlickablePrivate::_q_resume()
{
    if (!interactionEngine)
        return;

    pageIsSuspended = false;
    webPageProxy->resumeActiveDOMObjectsAndAnimations();

    if (isTransitioningToNewPage) {
        isTransitioningToNewPage = false;
        postTransitionState->apply();
    }

    _q_commitScaleChange();
}
