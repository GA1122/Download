std::string DownloadItemImpl::DebugString(bool verbose) const {
  std::string description =
      base::StringPrintf("{ id = %d"
                         " state = %s",
                         download_id_,
                         DebugDownloadStateString(state_));

  std::string url_list("<none>");
  if (!request_info_.url_chain.empty()) {
    std::vector<GURL>::const_iterator iter = request_info_.url_chain.begin();
    std::vector<GURL>::const_iterator last = request_info_.url_chain.end();
    url_list = (*iter).is_valid() ? (*iter).spec() : "<invalid>";
    ++iter;
    for ( ; verbose && (iter != last); ++iter) {
      url_list += " ->\n\t";
      const GURL& next_url = *iter;
      url_list += next_url.is_valid() ? next_url.spec() : "<invalid>";
    }
  }

  if (verbose) {
    description += base::StringPrintf(
        " total = %" PRId64 " received = %" PRId64
        " reason = %s"
        " paused = %c"
        " resume_mode = %s"
        " auto_resume_count = %d"
        " danger = %d"
        " all_data_saved = %c"
        " last_modified = '%s'"
        " etag = '%s'"
        " has_download_file = %s"
        " url_chain = \n\t\"%s\"\n\t"
        " current_path = \"%" PRFilePath
        "\"\n\t"
        " target_path = \"%" PRFilePath
        "\""
        " referrer = \"%s\""
        " site_url = \"%s\"",
        GetTotalBytes(), GetReceivedBytes(),
        DownloadInterruptReasonToString(last_reason_).c_str(),
        IsPaused() ? 'T' : 'F', DebugResumeModeString(GetResumeMode()),
        auto_resume_count_, GetDangerType(), AllDataSaved() ? 'T' : 'F',
        GetLastModifiedTime().c_str(), GetETag().c_str(),
        download_file_.get() ? "true" : "false", url_list.c_str(),
        GetFullPath().value().c_str(), GetTargetFilePath().value().c_str(),
        GetReferrerUrl().spec().c_str(), GetSiteUrl().spec().c_str());
  } else {
    description += base::StringPrintf(" url = \"%s\"", url_list.c_str());
  }

  description += " }";

  return description;
}
