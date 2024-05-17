void QQuickWebView::touchEvent(QTouchEvent* event)
{
    Q_D(QQuickWebView);
    if (d->m_dialogActive) {
        event->ignore();
        return;
    }

    bool lockingDisabled = flickableDirection() != AutoFlickDirection
                           || event->touchPoints().size() != 1
                           || width() >= contentWidth()
                           || height() >= contentHeight();

    if (!lockingDisabled)
        d->axisLocker.update(event);
    else
        d->axisLocker.reset();

    forceActiveFocus();
    d->pageView->eventHandler()->handleTouchEvent(event);
}
