std::vector<std::string> GetOutputNames(const std::vector<XID>& output_ids) {
  std::vector<std::string> names;
  Display* display = GetXDisplay();
  Window root_window = DefaultRootWindow(display);
  XRRScreenResources* screen_resources =
      XRRGetScreenResources(display, root_window);
  for (std::vector<XID>::const_iterator iter = output_ids.begin();
       iter != output_ids.end(); ++iter) {
    XRROutputInfo* output =
        XRRGetOutputInfo(display, screen_resources, *iter);
    names.push_back(std::string(output->name));
    XRRFreeOutputInfo(output);
  }
  XRRFreeScreenResources(screen_resources);
  return names;
}
