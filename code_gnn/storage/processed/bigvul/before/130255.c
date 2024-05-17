HRESULT DataObjectImpl::GetData(FORMATETC* format_etc, STGMEDIUM* medium) {
  if (is_aborting_)
    return DV_E_FORMATETC;

  StoredData::iterator iter = contents_.begin();
  while (iter != contents_.end()) {
    if ((*iter)->format_etc.cfFormat == format_etc->cfFormat &&
        (*iter)->format_etc.lindex == format_etc->lindex &&
        ((*iter)->format_etc.tymed & format_etc->tymed)) {
      if ((*iter)->medium) {
        DuplicateMedium((*iter)->format_etc.cfFormat, (*iter)->medium, medium);
      } else {
        if (in_drag_loop_)
          return DV_E_FORMATETC;

        bool wait_for_data = false;

        if (!in_async_mode_ || async_operation_started_)
          wait_for_data = true;

        if (!wait_for_data)
          return DV_E_FORMATETC;

        if (observer_)
          observer_->OnWaitForData();

        if ((*iter)->downloader.get()) {
          (*iter)->downloader->Start(this);
          if (!(*iter)->downloader->Wait()) {
            is_aborting_ = true;
            return DV_E_FORMATETC;
          }
        }

        return GetData(format_etc, medium);
      }
      return S_OK;
    }
    ++iter;
  }

  return DV_E_FORMATETC;
}
