void TabStripGtk::UpdateLoadingAnimations() {
  for (int i = 0, index = 0; i < GetTabCount(); ++i, ++index) {
    TabGtk* current_tab = GetTabAt(i);
    if (current_tab->closing()) {
      --index;
    } else {
      TabRendererGtk::AnimationState state;
      TabContents* contents = model_->GetTabContentsAt(index);
      if (!contents || !contents->web_contents()->IsLoading()) {
        state = TabGtk::ANIMATION_NONE;
      } else if (contents->web_contents()->IsWaitingForResponse()) {
        state = TabGtk::ANIMATION_WAITING;
      } else {
        state = TabGtk::ANIMATION_LOADING;
      }
      if (current_tab->ValidateLoadingAnimation(state)) {
        gfx::Rect favicon_bounds = current_tab->favicon_bounds();
        gtk_widget_queue_draw_area(tabstrip_.get(),
            favicon_bounds.x() + current_tab->x(),
            favicon_bounds.y() + current_tab->y(),
            favicon_bounds.width(),
            favicon_bounds.height());
      }
    }
  }
}
