std::string TestURLLoader::GetReachableAbsoluteURL(
    const std::string& file_name) {
  pp::Var document_url(
      pp::PASS_REF,
      testing_interface_->GetDocumentURL(instance_->pp_instance(),
                                         NULL));
  std::string url(document_url.AsString());
  std::string old_name("test_case.html");
  size_t index = url.find(old_name);
  ASSERT_NE(index, std::string::npos);
  url.replace(index, old_name.length(), file_name);
  return url;
}
