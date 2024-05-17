bool PDFiumEngine::ContinuePaint(int progressive_index,
                                 pp::ImageData* image_data) {
  DCHECK_GE(progressive_index, 0);
  DCHECK_LT(static_cast<size_t>(progressive_index), progressive_paints_.size());
  DCHECK(image_data);

#if defined(OS_LINUX)
  g_last_instance_id = client_->GetPluginInstance()->pp_instance();
#endif

  int rv;
  FPDF_BITMAP bitmap = progressive_paints_[progressive_index].bitmap;
  int page_index = progressive_paints_[progressive_index].page_index;
  DCHECK_GE(page_index, 0);
  DCHECK_LT(static_cast<size_t>(page_index), pages_.size());
  FPDF_PAGE page = pages_[page_index]->GetPage();

  last_progressive_start_time_ = base::Time::Now();
  if (bitmap) {
    rv = FPDF_RenderPage_Continue(page, static_cast<IFSDK_PAUSE*>(this));
  } else {
    pp::Rect dirty = progressive_paints_[progressive_index].rect;
    bitmap = CreateBitmap(dirty, image_data);
    int start_x, start_y, size_x, size_y;
    GetPDFiumRect(page_index, dirty, &start_x, &start_y, &size_x, &size_y);
    FPDFBitmap_FillRect(bitmap, start_x, start_y, size_x, size_y, 0xFFFFFFFF);
    rv = FPDF_RenderPageBitmap_Start(
        bitmap, page, start_x, start_y, size_x, size_y,
        current_rotation_,
        GetRenderingFlags(), static_cast<IFSDK_PAUSE*>(this));
    progressive_paints_[progressive_index].bitmap = bitmap;
  }
  return rv != FPDF_RENDER_TOBECOUNTINUED;
}
