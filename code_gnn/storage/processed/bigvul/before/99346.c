  virtual void RunWithParams(const Tuple1<int>& params) {
    int result = params.a;
    if (result == net::OK ||
        result == net::OK_FOR_SESSION_ONLY) {
      net::CookieOptions options;
      if (result == net::OK_FOR_SESSION_ONLY)
        options.set_force_session();
      context_->cookie_store()->SetCookieWithOptions(url_, cookie_line_,
                                                     options);
    } else {
      if (!context_->IsExternal()) {
        CallRenderViewHostResourceDelegate(
            render_process_id_, render_view_id_,
            &RenderViewHostDelegate::Resource::OnContentBlocked,
            CONTENT_SETTINGS_TYPE_COOKIES);
      }
    }
    delete this;
  }
