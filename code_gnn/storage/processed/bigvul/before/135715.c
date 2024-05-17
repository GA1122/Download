void GranularityStrategyTest::SetupVerticalAlign(String str1,
                                                 String str2,
                                                 String str3,
                                                 size_t sel_begin,
                                                 size_t sel_end) {
  SetInnerHTML(
      "<html>"
      "<head>"
      "<style>"
      "span {"
      "vertical-align:20px;"
      "}"
      "</style>"
      "</head>"
      "<body>"
      "<div id='mytext'></div>"
      "</body>"
      "</html>");

  SetupTextSpan(str1, str2, str3, sel_begin, sel_end);
}
