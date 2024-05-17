HRESULT STDMETHODCALLTYPE WebView::Drop(IDataObject* pDataObject, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect)
{
    if (m_dropTargetHelper)
        m_dropTargetHelper->Drop(pDataObject, (POINT*)&pt, *pdwEffect);

    m_dragData = 0;
    *pdwEffect = m_lastDropEffect;
    POINTL localpt = pt;
    ::ScreenToClient(m_window, (LPPOINT)&localpt);
    DragData data(pDataObject, IntPoint(localpt.x, localpt.y), IntPoint(pt.x, pt.y), keyStateToDragOperation(grfKeyState));
    m_page->performDragControllerAction(DragControllerActionPerformDrag, &data);
    return S_OK;
}
