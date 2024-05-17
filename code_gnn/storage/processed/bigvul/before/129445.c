error::Error GLES2DecoderImpl::HandleWaitSyncPointCHROMIUM(
    uint32 immediate_data_size, const cmds::WaitSyncPointCHROMIUM& c) {
  group_->mailbox_manager()->PullTextureUpdates();
  if (wait_sync_point_callback_.is_null())
    return error::kNoError;

  return wait_sync_point_callback_.Run(c.sync_point) ?
      error::kNoError : error::kDeferCommandUntilLater;
}
