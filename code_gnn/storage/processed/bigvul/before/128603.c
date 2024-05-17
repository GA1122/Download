void Instance::OnEvent(uint32 control_id, uint32 event_id, void* data) {
  if (event_id == Button::EVENT_ID_BUTTON_CLICKED ||
      event_id == Button::EVENT_ID_BUTTON_STATE_CHANGED) {
    switch (control_id) {
      case kFitToPageButtonId:
        UserMetricsRecordAction("PDF.FitToPageButton");
        SetZoom(ZOOM_FIT_TO_PAGE, 0);
        ZoomChanged(zoom_);
        break;
      case kFitToWidthButtonId:
        UserMetricsRecordAction("PDF.FitToWidthButton");
        SetZoom(ZOOM_FIT_TO_WIDTH, 0);
        ZoomChanged(zoom_);
        break;
      case kZoomOutButtonId:
      case kZoomInButtonId:
        UserMetricsRecordAction(control_id == kZoomOutButtonId ?
            "PDF.ZoomOutButton" : "PDF.ZoomInButton");
        SetZoom(ZOOM_SCALE, CalculateZoom(control_id));
        ZoomChanged(zoom_);
        break;
      case kSaveButtonId:
        UserMetricsRecordAction("PDF.SaveButton");
        SaveAs();
        break;
      case kPrintButtonId:
        UserMetricsRecordAction("PDF.PrintButton");
        Print();
        break;
    }
  }
  if (control_id == kThumbnailsId &&
      event_id == ThumbnailControl::EVENT_ID_THUMBNAIL_SELECTED) {
    int page = *static_cast<int*>(data);
    pp::Rect page_rc(engine_->GetPageRect(page));
    ScrollToY(static_cast<int>(page_rc.y() * zoom_ * device_scale_));
  }
}
