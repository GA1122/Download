void GLES2Implementation::IssueSetDisjointValueSync(
    uint32_t sync_data_shm_id,
    uint32_t sync_data_shm_offset) {
  helper_->SetDisjointValueSyncCHROMIUM(sync_data_shm_id, sync_data_shm_offset);
}
