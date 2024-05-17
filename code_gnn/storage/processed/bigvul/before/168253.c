bool BrowserView::GetAccelerator(int cmd_id,
                                 ui::Accelerator* accelerator) const {
#if defined(OS_MACOSX)
  if (GetDefaultMacAcceleratorForCommandId(cmd_id, accelerator))
    return true;
#else
  if (GetStandardAcceleratorForCommandId(cmd_id, accelerator))
    return true;
#endif
  for (std::map<ui::Accelerator, int>::const_iterator it =
           accelerator_table_.begin(); it != accelerator_table_.end(); ++it) {
    if (it->second == cmd_id) {
      *accelerator = it->first;
      return true;
    }
  }
  return false;
}
