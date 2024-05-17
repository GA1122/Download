LRESULT WebView::onGestureNotify(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, bool& handled)
{
    ASSERT(SetGestureConfigPtr());

    GESTURENOTIFYSTRUCT* gn = reinterpret_cast<GESTURENOTIFYSTRUCT*>(lParam);

    POINT localPoint = { gn->ptsLocation.x, gn->ptsLocation.y };
    ::ScreenToClient(m_window, &localPoint);

    bool canPan = m_page->gestureWillBegin(localPoint);

    DWORD dwPanWant = GC_PAN | GC_PAN_WITH_INERTIA | GC_PAN_WITH_GUTTER;
    DWORD dwPanBlock = GC_PAN_WITH_SINGLE_FINGER_HORIZONTALLY;
    if (canPan)
        dwPanWant |= GC_PAN_WITH_SINGLE_FINGER_VERTICALLY;
    else
        dwPanBlock |= GC_PAN_WITH_SINGLE_FINGER_VERTICALLY;

    GESTURECONFIG gc = { GID_PAN, dwPanWant, dwPanBlock };
    return SetGestureConfigPtr()(m_window, 0, 1, &gc, sizeof(gc));
}
