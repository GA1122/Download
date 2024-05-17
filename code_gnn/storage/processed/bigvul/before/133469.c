void OobeUI::OnCurrentScreenChanged(const std::string& screen) {
  previous_screen_ = current_screen_;
  DCHECK(screen_ids_.count(screen))
      << "Screen should be registered in InitializeScreenMaps()";
  Screen new_screen = screen_ids_[screen];
  FOR_EACH_OBSERVER(Observer,
                    observer_list_,
                    OnCurrentScreenChanged(current_screen_, new_screen));
  current_screen_ = new_screen;
}
