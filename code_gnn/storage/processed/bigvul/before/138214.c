AXObjectCacheImpl::AXObjectCacheImpl(Document& document)
    : m_document(document),
      m_modificationCount(0),
      m_notificationPostTimer(
          TaskRunnerHelper::get(TaskType::UnspecedTimer, &document),
          this,
          &AXObjectCacheImpl::notificationPostTimerFired) {}
