bool TemplateURLRef::EncodeFormData(const PostParams& post_params,
                                    PostContent* post_content) const {
  if (post_params.empty())
    return true;
  if (!post_content)
    return false;

  const char kUploadDataMIMEType[] = "multipart/form-data; boundary=";
  std::string boundary = net::GenerateMimeMultipartBoundary();
  post_content->first = kUploadDataMIMEType;
  post_content->first += boundary;
  std::string* post_data = &post_content->second;
  post_data->clear();
  for (const auto& param : post_params) {
    DCHECK(!param.name.empty());
    net::AddMultipartValueForUpload(param.name, param.value, boundary,
                                    param.content_type, post_data);
  }
  net::AddMultipartFinalDelimiterForUpload(boundary, post_data);
  return true;
}
