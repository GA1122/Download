void CopyOutputRequestCallback(base::Closure quit_closure,
                               gfx::Size expected_size,
                               std::unique_ptr<CopyOutputResult> result) {
  EXPECT_EQ(expected_size, result->size());
  quit_closure.Run();
}
