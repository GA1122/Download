    explicit OwnsReference(DataDeletionHelper* helper) : helper_(helper) {
      DCHECK_CURRENTLY_ON(BrowserThread::UI);
      helper->IncrementTaskCountOnUI();
    }
