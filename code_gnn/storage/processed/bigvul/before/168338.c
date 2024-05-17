void BrowserView::PaintChildren(const views::PaintInfo& paint_info) {
  views::ClientView::PaintChildren(paint_info);
  if (!histogram_helper_) {
    histogram_helper_ = BrowserWindowHistogramHelper::
        MaybeRecordValueAndCreateInstanceOnBrowserPaint(
            GetWidget()->GetCompositor());
  }
}
