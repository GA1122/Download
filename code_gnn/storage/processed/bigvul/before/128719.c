std::string TemplateURLRef::ParseURL(const std::string& url,
                                     Replacements* replacements,
                                     PostParams* post_params,
                                     bool* valid) const {
  *valid = false;
  std::string parsed_url = url;
  for (size_t last = 0; last != std::string::npos; ) {
    last = parsed_url.find(kStartParameter, last);
    if (last != std::string::npos) {
      size_t template_end = parsed_url.find(kEndParameter, last);
      if (template_end != std::string::npos) {
        size_t next_template_start = parsed_url.find(kStartParameter, last + 1);
        if (next_template_start == std::string::npos ||
            next_template_start > template_end) {
          if (!ParseParameter(last, template_end, &parsed_url, replacements)) {
            last = template_end;
          }
        } else {
          last = next_template_start;
        }
      } else {
        return std::string();
      }
    }
  }

  const std::string& post_params_string = GetPostParamsString();
  if (!post_params_string.empty()) {
    for (const base::StringPiece& cur : base::SplitStringPiece(
             post_params_string, ",",
             base::TRIM_WHITESPACE, base::SPLIT_WANT_ALL)) {
      std::vector<std::string> parts = base::SplitString(
          cur, "=", base::TRIM_WHITESPACE, base::SPLIT_WANT_ALL);
      if ((parts.size() != 2U) || parts[0].empty())
        return std::string();

      std::string& value = parts[1];
      size_t replacements_size = replacements->size();
      if (IsTemplateParameterString(value))
        ParseParameter(0, value.length() - 1, &value, replacements);
      PostParam param = { parts[0], value };
      post_params->push_back(param);
      if (replacements->size() > replacements_size) {
        DCHECK_EQ(replacements_size + 1, replacements->size());
        Replacement* r = &replacements->back();
        r->is_post_param = true;
        r->index = post_params->size() - 1;
      }
    }
    DCHECK(!post_params->empty());
  }

  *valid = true;
  return parsed_url;
}
