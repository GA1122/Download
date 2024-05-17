void GuestViewBase::WillAttach(content::WebContents* embedder_web_contents,
                               int element_instance_id,
                               bool is_full_page_plugin) {
  if (owner_web_contents_ != embedder_web_contents) {
    DCHECK_EQ(owner_contents_observer_->web_contents(), owner_web_contents_);
    StopTrackingEmbedderZoomLevel();
    owner_web_contents_ = embedder_web_contents;
    owner_contents_observer_.reset(
        new OwnerContentsObserver(this, embedder_web_contents));
  }

  StartTrackingEmbedderZoomLevel();
  element_instance_id_ = element_instance_id;
  is_full_page_plugin_ = is_full_page_plugin;

  WillAttachToEmbedder();
}
