void GetFilePathWithReplacements(const std::string& original_file_path,
                                 const base::StringPairs& text_to_replace,
                                 std::string* replacement_path) {
  std::string new_file_path = original_file_path;
  for (const auto& replacement : text_to_replace) {
    const std::string& old_text = replacement.first;
    const std::string& new_text = replacement.second;
    std::string base64_old;
    std::string base64_new;
    base::Base64Encode(old_text, &base64_old);
    base::Base64Encode(new_text, &base64_new);
    if (new_file_path == original_file_path)
      new_file_path += "?";
    else
      new_file_path += "&";
    new_file_path += "replace_text=";
    new_file_path += base64_old;
    new_file_path += ":";
    new_file_path += base64_new;
  }

   *replacement_path = new_file_path;
 }
