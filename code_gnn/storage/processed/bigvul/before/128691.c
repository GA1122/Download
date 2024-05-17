bool FindSearchTermsInPath(const std::string& path,
                           url::Component* parameter_position) {
  DCHECK(parameter_position);
  parameter_position->reset();
  const size_t begin = path.find(kSearchTermsParameterFullEscaped);
  if (begin == std::string::npos)
    return false;
  parameter_position->begin = begin;
  parameter_position->len = arraysize(kSearchTermsParameterFullEscaped) - 1;
  return true;
}
