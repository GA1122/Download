void WebContentsImpl::ShowCreatedWindow(int route_id,
                                        WindowOpenDisposition disposition,
                                        const gfx::Rect& initial_rect,
                                        bool user_gesture) {
  WebContentsImpl* contents = GetCreatedWindow(route_id);
  if (contents) {
    WebContentsDelegate* delegate = GetDelegate();
    contents->is_resume_pending_ = true;
    if (!delegate || delegate->ShouldResumeRequestsForCreatedWindow())
      contents->ResumeLoadingCreatedWebContents();

    if (delegate) {
      delegate->AddNewContents(
          this, contents, disposition, initial_rect, user_gesture, NULL);
    }
  }
}
