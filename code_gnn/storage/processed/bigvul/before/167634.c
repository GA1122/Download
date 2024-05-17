  void OnUpdateViewportIntersection(const gfx::Rect& viewport_intersection,
                                    const gfx::Rect& compositing_rect) {
    content::BrowserThread::PostTask(
        content::BrowserThread::IO, FROM_HERE,
        base::BindOnce(&UpdateViewportIntersectionMessageFilter::
                           OnUpdateViewportIntersectionPostOnIO,
                       this, viewport_intersection, compositing_rect));
  }
