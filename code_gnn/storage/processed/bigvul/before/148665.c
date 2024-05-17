SkiaOutputSurfaceImpl::ScopedPaint::ScopedPaint(
    SkDeferredDisplayListRecorder* root_recorder)
    : recorder_(root_recorder), render_pass_id_(0) {}
