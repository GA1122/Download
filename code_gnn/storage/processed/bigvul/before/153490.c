TabStrip::TabStrip(std::unique_ptr<TabStripController> controller)
    : controller_(std::move(controller)),
      layout_helper_(std::make_unique<TabStripLayoutHelper>()),
      drag_context_(std::make_unique<TabDragContextImpl>(this)) {
  Init();
  SetEventTargeter(std::make_unique<views::ViewTargeter>(this));
  md_observer_.Add(MD::GetInstance());
}
