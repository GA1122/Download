HRESULT DataObjectImpl::SetData(
    FORMATETC* format_etc, STGMEDIUM* medium, BOOL should_release) {
  RemoveData(*format_etc);

  STGMEDIUM* local_medium = new STGMEDIUM;
  if (should_release) {
    *local_medium = *medium;
  } else {
    DuplicateMedium(format_etc->cfFormat, medium, local_medium);
  }

  DataObjectImpl::StoredDataInfo* info =
      new DataObjectImpl::StoredDataInfo(*format_etc, local_medium);
  info->medium->tymed = format_etc->tymed;
  info->owns_medium = !!should_release;
  contents_.insert(contents_.begin(), info);

  return S_OK;
}
