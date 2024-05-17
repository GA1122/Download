void TestingAutomationProvider::AwaitSyncRestart(
    Browser* browser,
    DictionaryValue* args,
    IPC::Message* reply_message) {
  AutomationJSONReply reply(this, reply_message);
  if (sync_waiter_.get() == NULL) {
    sync_waiter_.reset(
        ProfileSyncServiceHarness::CreateAndAttach(browser->profile()));
  }
  if (!sync_waiter_->IsSyncAlreadySetup()) {
    reply.SendError("Not signed in to sync");
    return;
  }
  if (!browser->profile()->GetProfileSyncService()) {
    reply.SendError("ProfileSyncService initialization failed.");
    return;
  }
  if (!sync_waiter_->AwaitSyncRestart()) {
    reply.SendError("Sync did not successfully restart.");
    return;
  }
  ProfileSyncService::Status status = sync_waiter_->GetStatus();
  if (status.summary == ProfileSyncService::Status::READY) {
    scoped_ptr<DictionaryValue> return_value(new DictionaryValue);
    return_value->SetBoolean("success", true);
    reply.SendSuccess(return_value.get());
  } else {
    std::string error_msg = "Wait for sync restart was unsuccessful. "
                            "Sync status: ";
    error_msg.append(
        ProfileSyncService::BuildSyncStatusSummaryText(status.summary));
    reply.SendError(error_msg);
  }
}
