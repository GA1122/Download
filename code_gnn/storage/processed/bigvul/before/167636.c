  void OnUpdateViewportIntersectionPostOnIO(
      const gfx::Rect& viewport_intersection,
      const gfx::Rect& compositing_rect) {
    content::BrowserThread::PostTask(
        content::BrowserThread::UI, FROM_HERE,
        base::BindOnce(&UpdateViewportIntersectionMessageFilter::
                           OnUpdateViewportIntersectionOnUI,
                       this, viewport_intersection, compositing_rect));
  }
