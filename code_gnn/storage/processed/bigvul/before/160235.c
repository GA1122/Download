static void ExpectResultAndRun(T expected,
                               const base::Closure& continuation,
                               T actual) {
  EXPECT_EQ(expected, actual);
  continuation.Run();
}
