void BookmarkBarViewBackground::Paint(gfx::Canvas* canvas,
                                      views::View* view) const {
  int toolbar_overlap = bookmark_bar_view_->GetToolbarOverlap();

  SkAlpha detached_alpha = static_cast<SkAlpha>(
      bookmark_bar_view_->size_animation().CurrentValueBetween(0xff, 0));
  if (detached_alpha != 0xff) {
    PaintAttachedBookmarkBar(canvas, bookmark_bar_view_, browser_view_,
                             toolbar_overlap);
  }

  if (!bookmark_bar_view_->IsDetached() || detached_alpha == 0)
    return;

  canvas->SaveLayerAlpha(detached_alpha);
  PaintDetachedBookmarkBar(canvas, bookmark_bar_view_);
  canvas->Restore();
}
