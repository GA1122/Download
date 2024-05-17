void SyncBackendHost::Core::FinishConfigureDataTypes() {
  DCHECK_EQ(MessageLoop::current(), sync_loop_);
  host_.Call(
      FROM_HERE,
      &SyncBackendHost::FinishConfigureDataTypesOnFrontendLoop);
}
