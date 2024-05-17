error::Error GLES2DecoderImpl::HandleSetDisjointValueSyncCHROMIUM(
    uint32_t immediate_data_size,
    const volatile void* cmd_data) {
  const volatile gles2::cmds::SetDisjointValueSyncCHROMIUM& c =
      *static_cast<const volatile gles2::cmds::SetDisjointValueSyncCHROMIUM*>(
          cmd_data);
  int32_t sync_shm_id = static_cast<int32_t>(c.sync_data_shm_id);
  uint32_t sync_shm_offset = static_cast<uint32_t>(c.sync_data_shm_offset);

  return query_manager_->SetDisjointSync(sync_shm_id, sync_shm_offset);
}
