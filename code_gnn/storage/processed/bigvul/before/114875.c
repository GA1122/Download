void TestingAutomationProvider::EnableSyncForDatatypes(
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
  ListValue* datatypes = NULL;
  if (!args->GetList("datatypes", &datatypes)) {
    reply.SendError("Invalid or missing args");
    return;
  }
  std::string first_datatype;
  datatypes->GetString(0, &first_datatype);
  if (first_datatype == "All") {
    sync_waiter_->EnableSyncForAllDatatypes();
  } else {
    int num_datatypes = datatypes->GetSize();
    for (int i = 0; i < num_datatypes; ++i) {
      std::string datatype_string;
      datatypes->GetString(i, &datatype_string);
      syncable::ModelType datatype =
          syncable::ModelTypeFromString(datatype_string);
      if (datatype == syncable::UNSPECIFIED) {
        AutomationJSONReply(this, reply_message).SendError(StringPrintf(
            "Invalid datatype string: %s.", datatype_string.c_str()));
        return;
      }
      sync_waiter_->EnableSyncForDatatype(datatype);
      sync_waiter_->AwaitFullSyncCompletion(StringPrintf(
          "Enabling datatype: %s", datatype_string.c_str()));
    }
  }
  ProfileSyncService::Status status = sync_waiter_->GetStatus();
  if (status.summary == ProfileSyncService::Status::READY ||
      status.summary == ProfileSyncService::Status::SYNCING) {
    scoped_ptr<DictionaryValue> return_value(new DictionaryValue);
    return_value->SetBoolean("success", true);
    reply.SendSuccess(return_value.get());
  } else {
    reply.SendError("Enabling sync for given datatypes was unsuccessful");
  }
}
