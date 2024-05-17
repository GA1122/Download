void RunElisionParsingTest(const std::vector<ParsingTestcase>& testcases) {
  const gfx::FontList font_list;
  for (const auto& testcase : testcases) {
    SCOPED_TRACE(testcase.input + " to " + testcase.output);

    const GURL url(testcase.input);
    const float available_width =
        gfx::GetStringWidthF(base::UTF8ToUTF16(testcase.output), font_list);

    url::Parsed parsed;
    auto elided =
        url_formatter::ElideUrlSimple(url, font_list, available_width, &parsed);
    EXPECT_EQ(base::UTF8ToUTF16(testcase.output), elided);

    url::Parsed expected;
    for (const auto& expectation : testcase.components) {
      url::Component* component = GetComponent(&expected, expectation.type);
      component->begin = expectation.begin;
      component->len = expectation.len;
    }

    const std::vector<url::Parsed::ComponentType> kComponents = {
        url::Parsed::SCHEME, url::Parsed::USERNAME, url::Parsed::PASSWORD,
        url::Parsed::HOST,   url::Parsed::PORT,     url::Parsed::PATH,
        url::Parsed::QUERY,  url::Parsed::REF,
    };
    for (const auto& type : kComponents) {
      EXPECT_EQ(GetComponent(&expected, type)->begin,
                GetComponent(&parsed, type)->begin)
          << " in component " << type;
      EXPECT_EQ(GetComponent(&expected, type)->len,
                GetComponent(&parsed, type)->len)
          << " in component " << type;
    }
  }
}
