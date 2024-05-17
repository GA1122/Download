  ChromePluginPrintContext(LocalFrame* frame,
                           WebPluginContainerImpl* plugin,
                           const WebPrintParams& print_params)
      : ChromePrintContext(frame),
        plugin_(plugin),
        print_params_(print_params) {}
