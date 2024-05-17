SessionService::SessionService(const FilePath& save_path)
    : BaseSessionService(SESSION_RESTORE, NULL, save_path),
      has_open_trackable_browsers_(false),
      move_on_new_browser_(false),
      save_delay_in_millis_(base::TimeDelta::FromMilliseconds(2500)),
      save_delay_in_mins_(base::TimeDelta::FromMinutes(10)),
      save_delay_in_hrs_(base::TimeDelta::FromHours(8)) {
  Init();
}
