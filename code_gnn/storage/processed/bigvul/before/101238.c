bool SyncerProtoUtil::PostClientToServerMessage(
    const ClientToServerMessage& msg,
    ClientToServerResponse* response,
    SyncSession* session) {

  CHECK(response);
  DCHECK(!msg.get_updates().has_from_timestamp());   
  DCHECK(!msg.get_updates().has_requested_types());   
  DCHECK(msg.has_store_birthday() || IsVeryFirstGetUpdates(msg))
      << "Must call AddRequestBirthday to set birthday.";

  ScopedDirLookup dir(session->context()->directory_manager(),
      session->context()->account_name());
  if (!dir.good())
    return false;

  if (!PostAndProcessHeaders(session->context()->connection_manager(), session,
                             msg, response))
    return false;

  browser_sync::SyncProtocolError sync_protocol_error;

  if (!VerifyResponseBirthday(dir, response)) {
    sync_protocol_error.error_type = browser_sync::NOT_MY_BIRTHDAY;
     sync_protocol_error.action =
         browser_sync::DISABLE_SYNC_ON_CLIENT;
  } else if (response->has_error()) {
    sync_protocol_error = ConvertErrorPBToLocalType(response->error());
  } else {
    sync_protocol_error = ConvertLegacyErrorCodeToNewError(
        response->error_code());
  }

  sessions::StatusController* status = session->status_controller();
  status->set_sync_protocol_error(sync_protocol_error);

  session->delegate()->OnSyncProtocolError(session->TakeSnapshot());

  switch (sync_protocol_error.error_type) {
    case browser_sync::UNKNOWN_ERROR:
      LOG(WARNING) << "Sync protocol out-of-date. The server is using a more "
                   << "recent version.";
      return false;
    case browser_sync::SYNC_SUCCESS:
      LogResponseProfilingData(*response);
      return true;
    case browser_sync::THROTTLED:
      LOG(WARNING) << "Client silenced by server.";
      session->delegate()->OnSilencedUntil(base::TimeTicks::Now() +
          GetThrottleDelay(*response));
      return false;
    case browser_sync::TRANSIENT_ERROR:
      return false;
    case browser_sync::MIGRATION_DONE:
      HandleMigrationDoneResponse(response, session);
      return false;
    case browser_sync::CLEAR_PENDING:
    case browser_sync::NOT_MY_BIRTHDAY:
      return false;
    default:
      NOTREACHED();
      return false;
  }
}
