  void ResetViewportToAnchoredPosition() {
    DCHECK(outer_);

    inner_->ClampScrollToMaxScrollOffset();
    outer_->ClampScrollToMaxScrollOffset();

    gfx::ScrollOffset viewport_location =
        inner_->CurrentScrollOffset() + outer_->CurrentScrollOffset();

    gfx::Vector2dF delta =
        viewport_in_content_coordinates_.DeltaFrom(viewport_location);

    delta = inner_->ScrollBy(delta);
    outer_->ScrollBy(delta);
  }
