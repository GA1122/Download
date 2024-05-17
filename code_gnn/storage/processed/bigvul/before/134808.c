void TouchEventConverterEvdev::Initialize(const EventDeviceInfo& info) {
  has_mt_ = info.HasMultitouch();

  if (has_mt_) {
    pressure_min_ = info.GetAbsMinimum(ABS_MT_PRESSURE);
    pressure_max_ = info.GetAbsMaximum(ABS_MT_PRESSURE);
    x_min_tuxels_ = info.GetAbsMinimum(ABS_MT_POSITION_X);
    x_num_tuxels_ = info.GetAbsMaximum(ABS_MT_POSITION_X) - x_min_tuxels_ + 1;
    y_min_tuxels_ = info.GetAbsMinimum(ABS_MT_POSITION_Y);
    y_num_tuxels_ = info.GetAbsMaximum(ABS_MT_POSITION_Y) - y_min_tuxels_ + 1;
    touch_points_ =
        std::min<int>(info.GetAbsMaximum(ABS_MT_SLOT) + 1, kNumTouchEvdevSlots);
    current_slot_ = info.GetAbsValue(ABS_MT_SLOT);
  } else {
    pressure_min_ = info.GetAbsMinimum(ABS_PRESSURE);
    pressure_max_ = info.GetAbsMaximum(ABS_PRESSURE);
    x_min_tuxels_ = info.GetAbsMinimum(ABS_X);
    x_num_tuxels_ = info.GetAbsMaximum(ABS_X) - x_min_tuxels_ + 1;
    y_min_tuxels_ = info.GetAbsMinimum(ABS_Y);
    y_num_tuxels_ = info.GetAbsMaximum(ABS_Y) - y_min_tuxels_ + 1;
    touch_points_ = 1;
    current_slot_ = 0;
  }

  if (type() == INPUT_DEVICE_INTERNAL) {
    TouchCalibration cal = {};
    GetTouchCalibration(&cal);
    x_min_tuxels_ += cal.bezel_left;
    x_num_tuxels_ -= cal.bezel_left + cal.bezel_right;
    y_min_tuxels_ += cal.bezel_top;
    y_num_tuxels_ -= cal.bezel_top + cal.bezel_bottom;

    VLOG(1) << "applying touch calibration: "
            << base::StringPrintf("[%d, %d, %d, %d]", cal.bezel_left,
                                  cal.bezel_right, cal.bezel_top,
                                  cal.bezel_bottom);
  }

  events_.resize(touch_points_);

  if (has_mt_) {
    for (size_t i = 0; i < events_.size(); ++i) {
      events_[i].x = info.GetAbsMtSlotValueWithDefault(ABS_MT_POSITION_X, i, 0);
      events_[i].y = info.GetAbsMtSlotValueWithDefault(ABS_MT_POSITION_Y, i, 0);
      events_[i].tracking_id = info.GetAbsMtSlotValueWithDefault(
          ABS_MT_TRACKING_ID, i, kTrackingIdForUnusedSlot);
      events_[i].touching = (events_[i].tracking_id >= 0);
      events_[i].slot = i;

      events_[i].altered = true;

      events_[i].radius_x =
          info.GetAbsMtSlotValueWithDefault(ABS_MT_TOUCH_MAJOR, i, 0) / 2.0f;
      events_[i].radius_y =
          info.GetAbsMtSlotValueWithDefault(ABS_MT_TOUCH_MINOR, i, 0) / 2.0f;
      events_[i].pressure = ScalePressure(
          info.GetAbsMtSlotValueWithDefault(ABS_MT_PRESSURE, i, 0));
    }
  } else {
    events_[0].x = 0;
    events_[0].y = 0;
    events_[0].tracking_id = kTrackingIdForUnusedSlot;
    events_[0].touching = false;
    events_[0].slot = 0;
    events_[0].radius_x = 0;
    events_[0].radius_y = 0;
    events_[0].pressure = 0;
  }
}
