GURL MaybeStripReferrer(const GURL& possible_referrer) {
  if (CommandLine::ForCurrentProcess()->HasSwitch(switches::kNoReferrers))
    return GURL();
  return possible_referrer;
}
