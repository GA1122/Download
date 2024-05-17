base::TimeDelta GetTickDeltaSinceEpoch() {
  return NowTicks() - base::TimeTicks::UnixEpoch();
}
