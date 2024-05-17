int64 BuildCommitCommand::InterpolatePosition(const int64 lo,
                                              const int64 hi) {
  DCHECK_LE(lo, hi);

  if (lo == GetFirstPosition() && hi == GetLastPosition())
    return 0;

  uint64 delta = static_cast<uint64>(hi) - static_cast<uint64>(lo);
  if (delta <= static_cast<uint64>(GetGap()*2))
    return lo + (static_cast<int64>(delta) + 7) / 8;   
  else if (lo == GetFirstPosition())
    return hi - GetGap();   
  else
    return lo + GetGap();   
}
