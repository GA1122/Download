Skeletons GetSkeletons(const base::string16& host) {
  return g_idn_spoof_checker.Get().GetSkeletons(host);
}
