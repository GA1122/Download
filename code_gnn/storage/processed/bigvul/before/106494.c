void WebPageProxy::didPerformDragControllerAction(uint64_t resultOperation)
{
    m_currentDragOperation = static_cast<DragOperation>(resultOperation);
}
