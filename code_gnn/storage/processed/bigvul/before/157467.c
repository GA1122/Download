  static void Begin(const wchar_t* const protocols[],
                    const base::Closure& on_finished_callback) {
    delete instance_;
    instance_ = new OpenSystemSettingsHelper(protocols, on_finished_callback);
  }
