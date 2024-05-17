void DesktopWindowTreeHostX11::SwapNonClientEventHandler(
    std::unique_ptr<ui::EventHandler> handler) {
  wm::CompoundEventFilter* compound_event_filter =
      desktop_native_widget_aura_->root_window_event_filter();
  if (x11_non_client_event_filter_)
    compound_event_filter->RemoveHandler(x11_non_client_event_filter_.get());
  compound_event_filter->AddHandler(handler.get());
  x11_non_client_event_filter_ = std::move(handler);
}
