void MimeUtil::SplitCodecsToVector(const std::string& codecs,
                                   std::vector<std::string>* codecs_out,
                                   bool strip) {
  *codecs_out =
      base::SplitString(base::TrimString(codecs, "\"", base::TRIM_ALL), ",",
                        base::TRIM_WHITESPACE, base::SPLIT_WANT_ALL);

  if (codecs_out->size() == 1 && (*codecs_out)[0].empty())
    codecs_out->clear();

  if (!strip)
    return;

  for (std::vector<std::string>::iterator it = codecs_out->begin();
       it != codecs_out->end(); ++it) {
    size_t found = it->find_first_of('.');
    if (found != std::string::npos)
      it->resize(found);
  }
}
