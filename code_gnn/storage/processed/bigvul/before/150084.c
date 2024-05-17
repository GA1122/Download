::testing::AssertionResult AssertFrameTimeContained(
    const char* haystack_expr,
    const char* needle_expr,
    const std::vector<BeginFrameArgs> haystack,
    const BeginFrameArgs needle) {
  auto failure = ::testing::AssertionFailure()
                 << needle.frame_time << " (" << needle_expr
                 << ") not found in " << haystack_expr;

  if (haystack.size() == 0) {
    failure << " which is empty.";
  } else {
    failure << " which contains:\n";
    for (size_t i = 0; i < haystack.size(); i++) {
      if (haystack[i].frame_time == needle.frame_time)
        return ::testing::AssertionSuccess();
      failure << "  [" << i << "]: " << haystack[i].frame_time << "\n";
    }
  }

  return failure;
}
