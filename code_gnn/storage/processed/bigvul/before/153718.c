void GLES2Implementation::GetResultNameHelper(GLsizei bufsize,
                                              GLsizei* length,
                                              char* name) {
  GLsizei max_length = 0;
  if (name && (bufsize > 0)) {
    std::vector<int8_t> str;
    GetBucketContents(kResultBucketId, &str);
    if (!str.empty()) {
      DCHECK_LE(str.size(), static_cast<size_t>(INT_MAX));
      max_length = std::min(bufsize, static_cast<GLsizei>(str.size())) - 1;
    }
    memcpy(name, str.data(), max_length);
    name[max_length] = '\0';
  }
  if (length) {
    *length = max_length;
  }
}
