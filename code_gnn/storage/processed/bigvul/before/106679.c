HRESULT STDMETHODCALLTYPE WebView::DragLeave()
{
    if (m_dropTargetHelper)
        m_dropTargetHelper->DragLeave();

    if (m_dragData) {
        DragData data(m_dragData.get(), IntPoint(), IntPoint(), DragOperationNone);
        m_page->performDragControllerAction(DragControllerActionExited, &data);
        m_dragData = 0;
        m_page->resetDragOperation();
    }
    return S_OK;
}
