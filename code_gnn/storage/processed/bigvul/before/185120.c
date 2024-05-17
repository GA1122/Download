  EntrySync* EntrySync::moveTo(DirectoryEntrySync* parent, const String& name, ExceptionState& exceptionState) const
  {
    RefPtr<EntrySyncCallbackHelper> helper = EntrySyncCallbackHelper::create();
//     EntrySyncCallbackHelper* helper = EntrySyncCallbackHelper::create();
      m_fileSystem->move(this, parent, name, helper->successCallback(), helper->errorCallback(), DOMFileSystemBase::Synchronous);
      return helper->getResult(exceptionState);
  }