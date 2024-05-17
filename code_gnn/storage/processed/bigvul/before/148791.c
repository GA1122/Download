InterstitialPageImpl::InterstitialPageImpl(
    WebContents* web_contents,
    RenderWidgetHostDelegate* render_widget_host_delegate,
    bool new_navigation,
    const GURL& url,
    InterstitialPageDelegate* delegate)
    : underlying_content_observer_(web_contents, this),
      web_contents_(web_contents),
      controller_(static_cast<NavigationControllerImpl*>(
          &web_contents->GetController())),
      render_widget_host_delegate_(render_widget_host_delegate),
      url_(url),
      new_navigation_(new_navigation),
      should_discard_pending_nav_entry_(new_navigation),
      enabled_(true),
      action_taken_(NO_ACTION),
      render_view_host_(NULL),
      frame_tree_(base::MakeUnique<FrameTree>(
          new InterstitialPageNavigatorImpl(this, controller_),
          this,
          this,
          this,
          static_cast<WebContentsImpl*>(web_contents))),
      original_child_id_(web_contents->GetRenderProcessHost()->GetID()),
      original_rvh_id_(web_contents->GetRenderViewHost()->GetRoutingID()),
      should_revert_web_contents_title_(false),
      resource_dispatcher_host_notified_(false),
      rvh_delegate_view_(new InterstitialPageRVHDelegateView(this)),
      create_view_(true),
      pause_throbber_(false),
      delegate_(delegate),
      weak_ptr_factory_(this) {
  InitInterstitialPageMap();
}
