std::string PPAPITest::BuildQuery(const std::string& base,
                                  const std::string& test_case){
  return StringPrintf("%stestcase=%s", base.c_str(), test_case.c_str());
}
