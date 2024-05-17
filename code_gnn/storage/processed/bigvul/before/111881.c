std::string ProfileSyncService::GetExperimentNameForDataType(
    syncable::ModelType data_type) {
  switch (data_type) {
    case syncable::SESSIONS:
      return "sync-tabs";
    default:
      break;
  }
  NOTREACHED();
  return "";
}
