  ChromePluginPrintContext(LocalFrame* frame,
                           WebPluginContainerImpl* plugin,
                           const WebPrintParams& print_params)
      : ChromePrintContext(frame, print_params.use_printing_layout),
        plugin_(plugin),
        print_params_(print_params) {}
