void TestingAutomationProvider::GetSyncInfo(Browser* browser,
                                            DictionaryValue* args,
                                            IPC::Message* reply_message) {
  AutomationJSONReply reply(this, reply_message);
  DictionaryValue* sync_info = new DictionaryValue;
  scoped_ptr<DictionaryValue> return_value(new DictionaryValue);
  if (sync_waiter_.get() == NULL) {
    sync_waiter_.reset(
        ProfileSyncServiceHarness::CreateAndAttach(browser->profile()));
  }
  if (!sync_waiter_->IsSyncAlreadySetup()) {
    sync_info->SetString("summary", "SYNC DISABLED");
  } else {
    ProfileSyncService* service = sync_waiter_->service();
    ProfileSyncService::Status status = sync_waiter_->GetStatus();
    sync_info->SetString("summary",
        ProfileSyncService::BuildSyncStatusSummaryText(status.summary));
    sync_info->SetString("sync url", service->sync_service_url().host());
    sync_info->SetBoolean("authenticated", status.authenticated);
    sync_info->SetString("last synced", service->GetLastSyncedTimeString());
    sync_info->SetInteger("updates received", status.updates_received);
    ListValue* synced_datatype_list = new ListValue;
    const syncable::ModelTypeSet synced_datatypes =
        service->GetPreferredDataTypes();
    for (syncable::ModelTypeSet::Iterator it = synced_datatypes.First();
         it.Good(); it.Inc()) {
      synced_datatype_list->Append(
          new StringValue(syncable::ModelTypeToString(it.Get())));
    }
    sync_info->Set("synced datatypes", synced_datatype_list);
  }
  return_value->Set("sync_info", sync_info);
  reply.SendSuccess(return_value.get());
}
