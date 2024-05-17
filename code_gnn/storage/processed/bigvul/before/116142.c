void ResourceDispatcherHostImpl::NotifyOnUI(int type,
                                            int render_process_id,
                                            int render_view_id,
                                            T* detail) {
  RenderViewHostImpl* rvh =
      RenderViewHostImpl::FromID(render_process_id, render_view_id);
  if (rvh) {
    RenderViewHostDelegate* rvhd = rvh->GetDelegate();
    NotificationService::current()->Notify(
        type, Source<WebContents>(rvhd->GetAsWebContents()),
        Details<T>(detail));
  }
  delete detail;
}
