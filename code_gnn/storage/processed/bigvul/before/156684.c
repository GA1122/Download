  void OnUpdateViewportIntersection(const gfx::Rect& viewport_intersection,
                                    const gfx::Rect& compositing_rect,
                                    bool occluded_or_obscured) {
    base::PostTaskWithTraits(
        FROM_HERE, {content::BrowserThread::IO},
        base::BindOnce(&UpdateViewportIntersectionMessageFilter::
                           OnUpdateViewportIntersectionPostOnIO,
                       this, viewport_intersection, compositing_rect,
                       occluded_or_obscured));
  }
