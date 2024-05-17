void RunProgressiveElisionTest(
    const std::vector<ProgressiveTestcase>& testcases,
    ElisionMethod method) {
  const gfx::FontList font_list;
  for (const auto& testcase : testcases) {
    SCOPED_TRACE("Eliding " + testcase.input);
    const GURL url(testcase.input);

    ASSERT_FALSE(testcase.output.empty());
    float width = std::max(
        gfx::GetStringWidthF(base::UTF8ToUTF16(testcase.input), font_list),
        gfx::GetStringWidthF(base::UTF8ToUTF16(testcase.output.front()),
                             font_list));

    int mismatches = 0;
    const int kMaxConsecutiveMismatches = 3;
    for (size_t i = 0; i < testcase.output.size(); i++) {
      const auto& expected = testcase.output[i];
      base::string16 expected_utf16 = base::UTF8ToUTF16(expected);
      base::string16 elided = Elide(url, font_list, width, method);
      if (expected_utf16 != elided) {
        if (i > 0 && i < testcase.output.size() - 1 &&
            mismatches < kMaxConsecutiveMismatches) {
          mismatches++;
          continue;
        }
        EXPECT_EQ(expected_utf16, elided);
        break;
      }
      mismatches = 0;
      float new_width = gfx::GetStringWidthF(elided, font_list);
      EXPECT_LE(new_width, std::ceil(width)) << " at " << elided;
      width = new_width - 1.0f;
    }
  }
}
