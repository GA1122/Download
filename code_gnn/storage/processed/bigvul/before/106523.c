void WebPageProxy::executeUndoRedo(uint32_t action, bool& result)
{
    m_pageClient->executeUndoRedo(static_cast<UndoOrRedo>(action));
    result = true;
}
