void WebPageProxy::performDragControllerAction(DragControllerAction action, WebCore::DragData* dragData, const String& dragStorageName)
{
    if (!isValid())
        return;
#if PLATFORM(WIN)
    process()->send(Messages::WebPage::PerformDragControllerAction(action, dragData->clientPosition(), dragData->globalPosition(),
        dragData->draggingSourceOperationMask(), dragData->dragDataMap(), dragData->flags()), m_pageID);
#else
    process()->send(Messages::WebPage::PerformDragControllerAction(action, dragData->clientPosition(), dragData->globalPosition(), dragData->draggingSourceOperationMask(), dragStorageName, dragData->flags()), m_pageID);
#endif
}
