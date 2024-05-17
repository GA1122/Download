std::string HttpUtil::GenerateAcceptLanguageHeader(
    const std::string& raw_language_list) {
  const unsigned int kQvalueDecrement10 = 2;
  unsigned int qvalue10 = 10;
  StringTokenizer t(raw_language_list, ",");
  std::string lang_list_with_q;
  while (t.GetNext()) {
    std::string language = t.token();
    if (qvalue10 == 10) {
      lang_list_with_q = language;
    } else {
      DCHECK_LT(qvalue10, 10U);
      base::StringAppendF(&lang_list_with_q, ",%s;q=0.%d", language.c_str(),
                          qvalue10);
    }
    if (qvalue10 > kQvalueDecrement10)
      qvalue10 -= kQvalueDecrement10;
  }
  return lang_list_with_q;
}
