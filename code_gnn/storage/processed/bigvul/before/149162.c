void VerboseExpect(size_t expected,
                   size_t actual,
                   const std::string& original_url,
                   size_t position,
                   const base::string16& formatted_url) {
  EXPECT_EQ(expected, actual) << "Original URL: " << original_url
      << " (at char " << position << ")\nFormatted URL: " << formatted_url;
}
