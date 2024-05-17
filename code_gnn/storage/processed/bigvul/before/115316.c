  std::vector<gfx::Rect> GetPanelBounds(
      const std::vector<Panel*>& panels) {
    std::vector<gfx::Rect> bounds;
    for (size_t i = 0; i < panels.size(); i++)
      bounds.push_back(panels[i]->GetBounds());
    return bounds;
  }
