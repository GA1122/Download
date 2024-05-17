QuarantineFileResult QuarantineFile(const base::FilePath& file,
                                    const GURL& source_url,
                                    const GURL& referrer_url,
                                    const std::string& client_guid) {
  bool source_succeeded =
      source_url.is_valid() &&
      SetExtendedFileAttribute(file.value().c_str(), kSourceURLExtendedAttrName,
                               source_url.spec().c_str(),
                               source_url.spec().length(), 0);

  bool referrer_succeeded =
      !referrer_url.is_valid() ||
      SetExtendedFileAttribute(
          file.value().c_str(), kReferrerURLExtendedAttrName,
          referrer_url.spec().c_str(), referrer_url.spec().length(), 0);
  return source_succeeded && referrer_succeeded
             ? QuarantineFileResult::OK
             : QuarantineFileResult::ANNOTATION_FAILED;
}
