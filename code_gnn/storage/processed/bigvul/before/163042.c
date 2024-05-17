bool ProfilingProcessHost::ShouldProfileProcessType(int process_type) {
  switch (mode()) {
    case Mode::kAll:
      return true;

    case Mode::kMinimal:
      return (process_type == content::ProcessType::PROCESS_TYPE_GPU ||
              process_type == content::ProcessType::PROCESS_TYPE_BROWSER);

    case Mode::kGpu:
      return process_type == content::ProcessType::PROCESS_TYPE_GPU;

    case Mode::kBrowser:
      return process_type == content::ProcessType::PROCESS_TYPE_BROWSER;

    case Mode::kRendererSampling:
      return false;

    case Mode::kNone:
      return false;

    case Mode::kCount:
      {}
  }

  NOTREACHED();
  return false;
}
