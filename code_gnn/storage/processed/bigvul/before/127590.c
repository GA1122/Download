bool GetOutputDeviceHandles(std::vector<XID>* outputs) {
  DCHECK(outputs);
  outputs->clear();

  if (!IsRandRAvailable())
    return false;

  Display* display = GetXDisplay();

  Window root_window = DefaultRootWindow(display);
  XRRScreenResources* screen_resources =
      XRRGetScreenResources(display, root_window);
  for (int i = 0; i < screen_resources->noutput; ++i)
    outputs->push_back(screen_resources->outputs[i]);
  XRRFreeScreenResources(screen_resources);
  return true;
}
