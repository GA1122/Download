void SkiaOutputSurfaceImplTest::CheckSyncTokenOnGpuThread(
    const gpu::SyncToken& sync_token) {
  EXPECT_TRUE(
      gpu_service()->sync_point_manager()->IsSyncTokenReleased(sync_token));
  UnblockMainThread();
}
