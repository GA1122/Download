static void handleScrolling(unsigned character, WebPagePrivate* scroller)
{
    const int scrollFactor = 20;
    int dx = 0, dy = 0;
    switch (character) {
    case KEYCODE_LEFT:
        dx = -scrollFactor;
        break;
    case KEYCODE_RIGHT:
        dx = scrollFactor;
        break;
    case KEYCODE_UP:
        dy = -scrollFactor;
        break;
    case KEYCODE_DOWN:
        dy = scrollFactor;
        break;
    case KEYCODE_PG_UP:
        ASSERT(scroller);
        dy = scrollFactor - scroller->actualVisibleSize().height();
        break;
    case KEYCODE_PG_DOWN:
        ASSERT(scroller);
        dy = scroller->actualVisibleSize().height() - scrollFactor;
        break;
    }

    if (dx || dy) {
        ASSERT(scroller && scroller->m_mainFrame && scroller->m_mainFrame->view());
        IntPoint pos(scroller->scrollPosition() + IntSize(dx, dy));

        if (pos.x() < 0)
            pos.setX(0);
        if (pos.y() < 0)
            pos.setY(0);
        if (pos.x() + scroller->actualVisibleSize().width() > scroller->contentsSize().width())
            pos.setX(scroller->contentsSize().width() - scroller->actualVisibleSize().width());
        if (pos.y() + scroller->actualVisibleSize().height() > scroller->contentsSize().height())
            pos.setY(scroller->contentsSize().height() - scroller->actualVisibleSize().height());

        scroller->m_mainFrame->view()->setScrollPosition(pos);
        scroller->m_client->scrollChanged();
    }
}
