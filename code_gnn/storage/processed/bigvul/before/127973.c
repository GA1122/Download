AwContents::AwContents(scoped_ptr<WebContents> web_contents)
    : browser_view_renderer_(
          this,
          BrowserThread::GetMessageLoopProxyForThread(BrowserThread::UI),
          base::CommandLine::ForCurrentProcess()->HasSwitch(
              switches::kDisablePageVisibility)),
      web_contents_(web_contents.Pass()),
      renderer_manager_key_(GLViewRendererManager::GetInstance()->NullKey()) {
  base::subtle::NoBarrier_AtomicIncrement(&g_instance_count, 1);
  icon_helper_.reset(new IconHelper(web_contents_.get()));
  icon_helper_->SetListener(this);
  web_contents_->SetUserData(android_webview::kAwContentsUserDataKey,
                             new AwContentsUserData(this));
  browser_view_renderer_.RegisterWithWebContents(web_contents_.get());
  render_view_host_ext_.reset(
      new AwRenderViewHostExt(this, web_contents_.get()));

  permission_request_handler_.reset(
      new PermissionRequestHandler(this, web_contents_.get()));

  AwAutofillClient* autofill_manager_delegate =
      AwAutofillClient::FromWebContents(web_contents_.get());
  if (autofill_manager_delegate)
    InitAutofillIfNecessary(autofill_manager_delegate->GetSaveFormData());
  if (base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kSingleProcess)) {
    content::SynchronousCompositor::SetClientForWebContents(
        web_contents_.get(), &browser_view_renderer_);
  }
}
