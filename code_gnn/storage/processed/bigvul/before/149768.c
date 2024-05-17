void CopyOutputResultCallback(base::Closure quit_closure,
                              const gpu::SyncToken& expected_sync_token,
                              bool expected_is_lost,
                              const gpu::SyncToken& sync_token,
                              bool is_lost) {
  EXPECT_EQ(expected_sync_token, sync_token);
  EXPECT_EQ(expected_is_lost, is_lost);
  quit_closure.Run();
}
