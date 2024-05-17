std::unique_ptr<HeadlessWebContentsImpl> HeadlessWebContentsImpl::Create(
    HeadlessWebContents::Builder* builder) {
  content::WebContents::CreateParams create_params(builder->browser_context_,
                                                   nullptr);
  create_params.initial_size = builder->window_size_;

  std::unique_ptr<HeadlessWebContentsImpl> headless_web_contents =
      base::WrapUnique(new HeadlessWebContentsImpl(
          content::WebContents::Create(create_params),
          builder->browser_context_));

  if (builder->tab_sockets_allowed_) {
    headless_web_contents->headless_tab_socket_ =
        base::MakeUnique<HeadlessTabSocketImpl>(
            headless_web_contents->web_contents_.get());
    headless_web_contents->inject_mojo_services_into_isolated_world_ = true;

    builder->mojo_services_.emplace_back(
        TabSocket::Name_, base::Bind(&CreateTabSocketMojoServiceForContents));
  }

  headless_web_contents->mojo_services_ = std::move(builder->mojo_services_);
  headless_web_contents->begin_frame_control_enabled_ =
      builder->enable_begin_frame_control_;
  headless_web_contents->InitializeWindow(gfx::Rect(builder->window_size_));
  if (!headless_web_contents->OpenURL(builder->initial_url_))
    return nullptr;
  return headless_web_contents;
}
