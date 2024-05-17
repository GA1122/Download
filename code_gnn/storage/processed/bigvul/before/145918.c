  views::Widget* GetCalloutWidgetForPanel(aura::Window* panel) {
    PanelLayoutManager* manager = PanelLayoutManager::Get(panel);
    DCHECK(manager);
    PanelLayoutManager::PanelList::iterator found = std::find(
        manager->panel_windows_.begin(), manager->panel_windows_.end(), panel);
    DCHECK(found != manager->panel_windows_.end());
    DCHECK(found->callout_widget);
    return found->CalloutWidget();
  }
