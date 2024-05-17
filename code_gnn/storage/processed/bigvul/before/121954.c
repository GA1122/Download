AppListController::AppListController()
    : current_view_(NULL),
      view_delegate_(NULL),
      can_close_app_list_(true),
      regain_first_lost_focus_(false),
      preserving_focus_for_taskbar_menu_(false),
      weak_factory_(this) {}
