void BrowserChildProcessHostImpl::CopyFeatureAndFieldTrialFlags(
    base::CommandLine* cmd_line) {
  base::FieldTrialList::CopyFieldTrialStateToFlags(
      switches::kFieldTrialHandle, switches::kEnableFeatures,
      switches::kDisableFeatures, cmd_line);
}
