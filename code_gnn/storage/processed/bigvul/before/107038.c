void QQuickWebViewFlickablePrivate::pageDidRequestScroll(const QPoint& pos)
{
    if (isTransitioningToNewPage) {
        postTransitionState->position = pos;
        return;
    }

    interactionEngine->pagePositionRequest(pos);
}
