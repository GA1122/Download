  void OnUpdateViewportIntersection(const gfx::Rect& viewport_intersection,
                                    const gfx::Rect& compositing_rect) {
    content::BrowserThread::PostTask(
        content::BrowserThread::UI, FROM_HERE,
        base::BindOnce(&UpdateViewportIntersectionMessageFilter::
                           OnUpdateViewportIntersectionOnUI,
                       this, compositing_rect));
  }
