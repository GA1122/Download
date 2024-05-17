HRESULT STDMETHODCALLTYPE WebView::DragOver(DWORD grfKeyState, POINTL pt, DWORD* pdwEffect)
{
    if (m_dropTargetHelper)
        m_dropTargetHelper->DragOver((POINT*)&pt, *pdwEffect);

    if (m_dragData) {
        POINTL localpt = pt;
        ::ScreenToClient(m_window, (LPPOINT)&localpt);
        DragData data(m_dragData.get(), IntPoint(localpt.x, localpt.y), IntPoint(pt.x, pt.y), keyStateToDragOperation(grfKeyState));
        m_page->performDragControllerAction(DragControllerActionUpdated, &data);
        *pdwEffect = dragOperationToDragCursor(m_page->dragOperation());
    } else
        *pdwEffect = DROPEFFECT_NONE;

    m_lastDropEffect = *pdwEffect;
    return S_OK;
}
