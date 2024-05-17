bool PrintingContext::InitializeSettings(const DEVMODE& dev_mode,
                                         const std::wstring& new_device_name,
                                         const PRINTPAGERANGE* ranges,
                                         int number_ranges,
                                         bool selection_only) {
  skia::PlatformDevice::InitializeDC(context_);
  DCHECK(GetDeviceCaps(context_, CLIPCAPS));
  DCHECK(GetDeviceCaps(context_, RASTERCAPS) & RC_STRETCHDIB);
  DCHECK(GetDeviceCaps(context_, RASTERCAPS) & RC_BITMAP64);

  if (!(GetDeviceCaps(context_, RASTERCAPS) & RC_STRETCHDIB) ||
      !(GetDeviceCaps(context_, RASTERCAPS) & RC_BITMAP64)) {
    NOTREACHED();
    ResetSettings();
    return false;
  }

  DCHECK(!in_print_job_);
  DCHECK(context_);
  PageRanges ranges_vector;
  if (!selection_only) {
    ranges_vector.reserve(number_ranges);
    for (int i = 0; i < number_ranges; ++i) {
      PageRange range;
      range.from = ranges[i].nFromPage - 1;
      range.to = ranges[i].nToPage - 1;
      ranges_vector.push_back(range);
    }
  }
  settings_.Init(context_,
                 dev_mode,
                 ranges_vector,
                 new_device_name,
                 selection_only);
  return true;
}
