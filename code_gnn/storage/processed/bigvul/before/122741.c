BrowserPluginGuest::BrowserPluginGuest(
    int instance_id,
    WebContentsImpl* web_contents,
    const BrowserPluginHostMsg_CreateGuest_Params& params)
    : WebContentsObserver(web_contents),
      embedder_web_contents_(NULL),
      instance_id_(instance_id),
#if defined(OS_WIN)
      damage_buffer_size_(0),
      remote_damage_buffer_handle_(0),
#endif
      damage_buffer_scale_factor_(1.0f),
      pending_update_counter_(0),
      guest_hang_timeout_(
          base::TimeDelta::FromMilliseconds(kGuestHangTimeoutMs)),
      focused_(params.focused),
      visible_(params.visible),
      auto_size_enabled_(params.auto_size_params.enable),
      max_auto_size_(params.auto_size_params.max_size),
      min_auto_size_(params.auto_size_params.min_size) {
  DCHECK(web_contents);
}
