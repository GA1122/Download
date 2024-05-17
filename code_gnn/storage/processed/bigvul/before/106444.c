void WebPageProxy::canUndoRedo(uint32_t action, bool& result)
{
    result = m_pageClient->canUndoRedo(static_cast<UndoOrRedo>(action));
}
