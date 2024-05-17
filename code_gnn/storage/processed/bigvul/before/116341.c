    void handleStatusChanged(QQuickView::Status status)
    {
        if (status != QQuickView::Ready)
            return;

        setGeometry(0, 0, 800, 600);

        setResizeMode(QQuickView::SizeRootObjectToView);
        m_view->setParentItem(rootObject());
        QQmlProperty::write(m_view, "anchors.fill", qVariantFromValue(rootObject()));

        QWindowSystemInterface::handleWindowActivated(this);
        m_view->page()->setFocus(true);
    }
