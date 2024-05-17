bool ExternalProtocolDialog::Cancel() {
  ExternalProtocolHandler::RecordHandleStateMetrics(
      false  , ExternalProtocolHandler::BLOCK);

  return true;
}
