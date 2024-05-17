error::Error GLES2DecoderImpl::HandleWaitSyncPointCHROMIUM(
    uint32 immediate_data_size,
    const void* cmd_data) {
  const gles2::cmds::WaitSyncPointCHROMIUM& c =
      *static_cast<const gles2::cmds::WaitSyncPointCHROMIUM*>(cmd_data);
  uint32 sync_point = c.sync_point;
  if (wait_sync_point_callback_.is_null())
    return error::kNoError;

  return wait_sync_point_callback_.Run(sync_point) ?
      error::kNoError : error::kDeferCommandUntilLater;
}
