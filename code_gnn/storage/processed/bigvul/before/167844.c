  void UpdateExpectations(TestingAction action) {
    PermissionRequestManager::AutoResponseType response_type =
        PermissionRequestManager::DISMISS;
    switch (action) {
      case ACCEPT:
        response_type = PermissionRequestManager::ACCEPT_ALL;
        break;
      case CANCEL:
        response_type = PermissionRequestManager::DENY_ALL;
        break;
      case WAIT:
        response_type = PermissionRequestManager::NONE;
        break;
    }
    mock_permission_prompt_factory_->set_response_type(response_type);
  }
