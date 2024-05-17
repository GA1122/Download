static bool SniffForOfficeDocs(const char* content,
                               size_t size,
                               const GURL& url,
                               bool* have_enough_content,
                               std::string* result) {
  *have_enough_content &= TruncateSize(kBytesRequiredForOfficeMagic, &size);

  std::string office_version;
  if (!CheckForMagicNumbers(content, size, kOfficeMagicNumbers,
                            arraysize(kOfficeMagicNumbers), &office_version))
    return false;

  OfficeDocType type = DOC_TYPE_NONE;
  base::StringPiece url_path = url.path_piece();
  for (size_t i = 0; i < arraysize(kOfficeExtensionTypes); ++i) {
    if (url_path.length() < kOfficeExtensionTypes[i].extension_len)
      continue;

    base::StringPiece extension = url_path.substr(
        url_path.length() - kOfficeExtensionTypes[i].extension_len);
    if (base::EqualsCaseInsensitiveASCII(
            extension,
            base::StringPiece(kOfficeExtensionTypes[i].extension,
                              kOfficeExtensionTypes[i].extension_len))) {
      type = kOfficeExtensionTypes[i].doc_type;
      break;
    }
  }

  if (type == DOC_TYPE_NONE)
    return false;

  if (office_version == "CFB") {
    switch (type) {
      case DOC_TYPE_WORD:
        *result = "application/msword";
        return true;
      case DOC_TYPE_EXCEL:
        *result = "application/vnd.ms-excel";
        return true;
      case DOC_TYPE_POWERPOINT:
        *result = "application/vnd.ms-powerpoint";
        return true;
      case DOC_TYPE_NONE:
        NOTREACHED();
        return false;
    }
  } else if (office_version == "OOXML") {
    switch (type) {
      case DOC_TYPE_WORD:
        *result = "application/vnd.openxmlformats-officedocument."
                  "wordprocessingml.document";
        return true;
      case DOC_TYPE_EXCEL:
        *result = "application/vnd.openxmlformats-officedocument."
                  "spreadsheetml.sheet";
        return true;
      case DOC_TYPE_POWERPOINT:
        *result = "application/vnd.openxmlformats-officedocument."
                  "presentationml.presentation";
        return true;
      case DOC_TYPE_NONE:
        NOTREACHED();
        return false;
    }
  }

  NOTREACHED();
  return false;
}
