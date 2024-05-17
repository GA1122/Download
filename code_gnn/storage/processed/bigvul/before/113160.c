void Launcher::SetFocusCycler(internal::FocusCycler* focus_cycler) {
  delegate_view_->set_focus_cycler(focus_cycler);
  focus_cycler->AddWidget(widget_.get());
}
