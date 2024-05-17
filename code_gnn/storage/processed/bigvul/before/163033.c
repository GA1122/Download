ProfilingProcessHost::ProfilingProcessHost()
    : is_registered_(false),
      background_triggers_(this),
      mode_(Mode::kNone),
      profiled_renderer_(nullptr),
      always_sample_for_tests_(false) {}
