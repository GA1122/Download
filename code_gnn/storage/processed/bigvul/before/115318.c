  void MoveMouse(gfx::Point position) {
    PanelManager::GetInstance()->OnMouseMove(position);
    MessageLoopForUI::current()->RunAllPending();
  }
