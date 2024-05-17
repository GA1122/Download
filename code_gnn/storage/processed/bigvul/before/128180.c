DEFINE_TRACE(Notification)
{
    visitor->trace(m_asyncRunner);
    RefCountedGarbageCollectedEventTargetWithInlineData<Notification>::trace(visitor);
    ActiveDOMObject::trace(visitor);
}
