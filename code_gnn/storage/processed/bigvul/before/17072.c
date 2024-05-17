void OxideQQuickWebViewPrivate::completeConstruction() {
  Q_Q(OxideQQuickWebView);

  Q_ASSERT(construct_props_.data());

  if (construct_props_->new_view_request) {
    proxy_.reset(oxide::qt::WebViewProxy::create(
        this, contents_view_.data(), q,
        find_controller_.data(),
        security_status_.data(),
        construct_props_->new_view_request));
  }

  if (!proxy_) {
    construct_props_->new_view_request = nullptr;
    proxy_.reset(oxide::qt::WebViewProxy::create(
        this, contents_view_.data(), q,
        find_controller_.data(),
        security_status_.data(),
        construct_props_->context,
        construct_props_->incognito,
        construct_props_->restore_state,
        construct_props_->restore_type));
  }

  proxy_->messageHandlers().swap(construct_props_->message_handlers);

  proxy_->setLocationBarHeight(construct_props_->location_bar_height);
  proxy_->setLocationBarMode(construct_props_->location_bar_mode);
  proxy_->setLocationBarAnimated(construct_props_->location_bar_animated);

  if (!construct_props_->new_view_request) {
    if (construct_props_->load_html) {
      proxy_->loadHtml(construct_props_->html, construct_props_->url);
    } else if (!construct_props_->url.isEmpty()) {
      proxy_->setUrl(construct_props_->url);
    }
  }

  proxy_->setFullscreen(construct_props_->fullscreen);

  if (construct_props_->preferences) {
    proxy_->setPreferences(construct_props_->preferences);
  }

  emit q->rootFrameChanged();

  if (construct_props_->incognito != proxy_->incognito()) {
    emit q->incognitoChanged();
  }
  if (construct_props_->context != proxy_->context()) {
    if (construct_props_->context) {
      detachContextSignals(
          OxideQQuickWebContextPrivate::get(construct_props_->context));
    }
    attachContextSignals(
        OxideQQuickWebContextPrivate::get(
          qobject_cast<OxideQQuickWebContext*>(proxy_->context())));
    emit q->contextChanged();
  }

  emit q->editingCapabilitiesChanged();

  construct_props_.reset();
}
