pp::Resource OutOfProcessInstance::PrintPages(
    const PP_PrintPageNumberRange_Dev* page_ranges,
    uint32_t page_range_count) {
  if (!print_settings_.is_printing)
    return pp::Resource();

  print_settings_.print_pages_called_ = true;
  return engine_->PrintPages(page_ranges, page_range_count,
                             print_settings_.pepper_print_settings);
}
