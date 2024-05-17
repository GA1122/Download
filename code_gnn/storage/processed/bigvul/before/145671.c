void ExtensionViewGuest::DidCommitProvisionalLoadForFrame(
    content::RenderFrameHost* render_frame_host,
    const GURL& url,
    ui::PageTransition transition_type) {
  if (render_frame_host->GetParent())
    return;

  url_ = url;

  scoped_ptr<base::DictionaryValue> args(new base::DictionaryValue());
  args->SetString(guest_view::kUrl, url_.spec());
  DispatchEventToView(make_scoped_ptr(
      new GuestViewEvent(extensionview::kEventLoadCommit, std::move(args))));
}
