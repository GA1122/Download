static void AdjustLinuxOOMScore(const std::string& process_type) {
  const int kZygoteScore = 0;
  const int kScoreBump = 100;
  const int kRendererScore = chrome::kLowestRendererOomScore;
  const int kMiscScore = kRendererScore - kScoreBump;
  const int kPluginScore = kMiscScore - kScoreBump;
  int score = -1;

  DCHECK(kMiscScore > 0);
  DCHECK(kPluginScore > 0);

  if (process_type == switches::kPluginProcess ||
      process_type == switches::kPpapiPluginProcess) {
    score = kPluginScore;
  } else if (process_type == switches::kPpapiBrokerProcess) {
    score = kPluginScore + kScoreBump;
  } else if (process_type == switches::kUtilityProcess ||
             process_type == switches::kWorkerProcess ||
             process_type == switches::kGpuProcess ||
             process_type == switches::kServiceProcess) {
    score = kMiscScore;
  } else if (process_type == switches::kProfileImportProcess) {
    NOTIMPLEMENTED();
    score = kZygoteScore;
#ifndef DISABLE_NACL
  } else if (process_type == switches::kNaClLoaderProcess) {
    score = kPluginScore;
#endif
  } else if (process_type == switches::kZygoteProcess ||
             process_type.empty()) {
    score = kZygoteScore;
  } else if (process_type == switches::kExtensionProcess ||
             process_type == switches::kRendererProcess) {
    LOG(WARNING) << "process type '" << process_type << "' "
                 << "should be created through the zygote.";
    score = kRendererScore;
  } else {
    NOTREACHED() << "Unknown process type";
  }
  if (score > -1)
    base::AdjustOOMScore(base::GetCurrentProcId(), score);
}
