  void ResetMockUI() {
    mock_ui_.reset(new MockPageInfoUI());
    mock_ui_->set_permission_info_callback_ =
        base::Bind(&PageInfoTest::SetPermissionInfo, base::Unretained(this));
  }
