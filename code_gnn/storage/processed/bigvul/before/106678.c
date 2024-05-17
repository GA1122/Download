HRESULT STDMETHODCALLTYPE WebView::DragEnter(IDataObject* pDataObject, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect)
{
    m_dragData = 0;
    m_page->resetDragOperation();

    if (m_dropTargetHelper)
        m_dropTargetHelper->DragEnter(m_window, pDataObject, (POINT*)&pt, *pdwEffect);

    POINTL localpt = pt;
    ::ScreenToClient(m_window, (LPPOINT)&localpt);
    DragData data(pDataObject, IntPoint(localpt.x, localpt.y), IntPoint(pt.x, pt.y), keyStateToDragOperation(grfKeyState));
    m_page->performDragControllerAction(DragControllerActionEntered, &data);
    *pdwEffect = dragOperationToDragCursor(m_page->dragOperation());

    m_lastDropEffect = *pdwEffect;
    m_dragData = pDataObject;

    return S_OK;
}
