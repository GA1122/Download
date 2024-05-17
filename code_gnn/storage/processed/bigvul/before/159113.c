DownloadItemImpl::DownloadItemImpl(DownloadItemImplDelegate* delegate,
                                   uint32_t download_id,
                                   const DownloadCreateInfo& info)
    : request_info_(info.url_chain,
                    info.referrer_url,
                    info.site_url,
                    info.tab_url,
                    info.tab_referrer_url,
                    base::UTF16ToUTF8(info.save_info->suggested_name),
                    info.save_info->file_path,
                    info.transition_type ? info.transition_type.value()
                                         : ui::PAGE_TRANSITION_LINK,
                    info.has_user_gesture,
                    info.remote_address,
                    info.start_time),
      guid_(info.guid.empty() ? base::GenerateGUID() : info.guid),
      download_id_(download_id),
      response_headers_(info.response_headers),
      content_disposition_(info.content_disposition),
      mime_type_(info.mime_type),
      original_mime_type_(info.original_mime_type),
      total_bytes_(info.total_bytes),
      last_reason_(info.result),
      start_tick_(base::TimeTicks::Now()),
      state_(INITIAL_INTERNAL),
      delegate_(delegate),
      is_temporary_(!info.transient && !info.save_info->file_path.empty()),
      transient_(info.transient),
      destination_info_(info.save_info->prompt_for_save_location
                            ? TARGET_DISPOSITION_PROMPT
                            : TARGET_DISPOSITION_OVERWRITE),
      last_modified_time_(info.last_modified),
      etag_(info.etag),
      is_updating_observers_(false),
      fetch_error_body_(info.fetch_error_body),
      weak_ptr_factory_(this) {
  delegate_->Attach();
  Init(true  , TYPE_ACTIVE_DOWNLOAD);

  TRACE_EVENT_INSTANT0("download", "DownloadStarted", TRACE_EVENT_SCOPE_THREAD);
}
