  WindowController::TypeFilter type_filters() {
    if (params_->get_info.get() && params_->get_info->window_types.get())
      return WindowController::GetFilterFromWindowTypes(
          *params_->get_info->window_types);
    return WindowController::kNoWindowFilter;
  }
