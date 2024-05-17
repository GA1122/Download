void RenderViewTest::OnLeakDetectionComplete(const Result& result) {
  EXPECT_EQ(0u, result.number_of_live_audio_nodes);
  EXPECT_EQ(0u, result.number_of_live_documents);
  EXPECT_EQ(0u, result.number_of_live_nodes);
  EXPECT_EQ(0u, result.number_of_live_layout_objects);
  EXPECT_EQ(0u, result.number_of_live_resources);
  EXPECT_EQ(0u, result.number_of_live_pausable_objects);
  EXPECT_EQ(0u, result.number_of_live_script_promises);
  EXPECT_EQ(0u, result.number_of_live_frames);
  EXPECT_EQ(0u, result.number_of_live_v8_per_context_data);
  EXPECT_EQ(0u, result.number_of_worker_global_scopes);
}
