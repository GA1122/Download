GetUpdatesCallerInfo::GetUpdatesSource GetSourceFromReason(
    sync_api::ConfigureReason reason) {
  switch (reason) {
    case sync_api::CONFIGURE_REASON_RECONFIGURATION:
      return GetUpdatesCallerInfo::RECONFIGURATION;
    case sync_api::CONFIGURE_REASON_MIGRATION:
      return GetUpdatesCallerInfo::MIGRATION;
    case sync_api::CONFIGURE_REASON_NEW_CLIENT:
      return GetUpdatesCallerInfo::NEW_CLIENT;
    case sync_api::CONFIGURE_REASON_NEWLY_ENABLED_DATA_TYPE:
      return GetUpdatesCallerInfo::NEWLY_SUPPORTED_DATATYPE;
    default:
      NOTREACHED();
  }

  return GetUpdatesCallerInfo::UNKNOWN;
}
