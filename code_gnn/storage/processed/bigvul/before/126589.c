void TabStripGtk::TabInsertedAt(WebContents* contents,
                                int index,
                                bool foreground) {
  TRACE_EVENT0("ui::gtk", "TabStripGtk::TabInsertedAt");

  DCHECK(contents);
  DCHECK(index == TabStripModel::kNoTab || model_->ContainsIndex(index));

  StopAnimation();

  bool contains_tab = false;
  TabGtk* tab = NULL;
  if (IsDragSessionActive()) {
    tab = drag_controller_->GetDraggedTabForContents(contents);
    if (tab) {
      tab->set_closing(false);
      tab->ValidateLoadingAnimation(TabRendererGtk::ANIMATION_NONE);
      tab->SetVisible(true);
    }

    std::vector<TabData>::const_iterator iter = tab_data_.begin();
    for (; iter != tab_data_.end() && !contains_tab; ++iter) {
      if (iter->tab == tab)
        contains_tab = true;
    }
  }

  if (!tab)
    tab = new TabGtk(this);

  if (!contains_tab) {
    TabData d = { tab, gfx::Rect() };
    tab_data_.insert(tab_data_.begin() + index, d);
    tab->UpdateData(contents, model_->IsAppTab(index), false);
  }
  tab->set_mini(model_->IsMiniTab(index));
  tab->set_app(model_->IsAppTab(index));
  tab->SetBlocked(model_->IsTabBlocked(index));

  if (gtk_widget_get_parent(tab->widget()) != tabstrip_.get())
    gtk_fixed_put(GTK_FIXED(tabstrip_.get()), tab->widget(), 0, 0);

  if (GetTabCount() > 1) {
    StartInsertTabAnimation(index);
    active_animation_->AnimationProgressed(NULL);
  } else {
    Layout();
  }

   ReStack();
 }
