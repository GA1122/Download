void QQuickWebView::touchEvent(QTouchEvent* event)
{
    Q_D(QQuickWebView);
    if (d->m_dialogRunnerActive) {
        event->ignore();
        return;
    }

    forceActiveFocus();
    d->pageView->eventHandler()->handleTouchEvent(event);
}
