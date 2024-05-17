  virtual void OnProgressUpdate(
      const GDataOperationRegistry::ProgressStatusList& list) {
    std::vector<ash::DriveOperationStatus> ui_list = GetDriveStatusList(list);
    NotifyRefreshDrive(ui_list);

    if (list.size() > 0) {
      bool has_in_progress_items = false;
      for (GDataOperationRegistry::ProgressStatusList::const_iterator it =
               list.begin();
          it != list.end(); ++it) {
        if (it->transfer_state ==
                GDataOperationRegistry::OPERATION_STARTED ||
            it->transfer_state ==
                GDataOperationRegistry::OPERATION_IN_PROGRESS ||
            it->transfer_state ==
                GDataOperationRegistry::OPERATION_SUSPENDED) {
          has_in_progress_items = true;
          break;
        }
      }

      if (!has_in_progress_items) {
        content::BrowserThread::PostDelayedTask(
            content::BrowserThread::UI,
            FROM_HERE,
            base::Bind(&SystemTrayDelegate::RecheckGDataOperations,
                       ui_weak_ptr_factory_->GetWeakPtr()),
            base::TimeDelta::FromMilliseconds(kGDataOperationRecheckDelayMs));
      }
    }

  }
