bool RenderViewHostImpl::CreateRenderView(
    const string16& frame_name,
    int opener_route_id,
    int32 max_page_id) {
  DCHECK(!IsRenderViewLive()) << "Creating view twice";

  if (!GetProcess()->Init())
    return false;
  DCHECK(GetProcess()->HasConnection());
  DCHECK(GetProcess()->GetBrowserContext());

  renderer_initialized_ = true;

  GpuSurfaceTracker::Get()->SetSurfaceHandle(
      surface_id(), GetCompositingSurface());

  int32 next_page_id = 1;
  if (max_page_id > -1)
    next_page_id = max_page_id + 1;

  ViewMsg_New_Params params;
  params.renderer_preferences =
      delegate_->GetRendererPrefs(GetProcess()->GetBrowserContext());
  params.web_preferences = delegate_->GetWebkitPrefs();
  params.view_id = GetRoutingID();
  params.surface_id = surface_id();
  params.session_storage_namespace_id = session_storage_namespace_->id();
  params.frame_name = frame_name;
  params.opener_route_id = opener_route_id;
  params.swapped_out = is_swapped_out_;
  params.next_page_id = next_page_id;
  GetWebScreenInfo(&params.screen_info);

  params.accessibility_mode =
      BrowserAccessibilityStateImpl::GetInstance()->GetAccessibilityMode();

  Send(new ViewMsg_New(params));

  if (GetProcess()->IsGuest())
    DCHECK_EQ(0, enabled_bindings_);
  Send(new ViewMsg_AllowBindings(GetRoutingID(), enabled_bindings_));
  delegate_->RenderViewCreated(this);

  FOR_EACH_OBSERVER(
      RenderViewHostObserver, observers_, RenderViewHostInitialized());

  return true;
}
