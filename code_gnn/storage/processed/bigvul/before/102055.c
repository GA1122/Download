void SyncBackendHost::Core::FinishConfigureDataTypes() {
  host_->frontend_loop_->PostTask(FROM_HERE, NewRunnableMethod(this,
      &SyncBackendHost::Core::FinishConfigureDataTypesOnFrontendLoop));
}
