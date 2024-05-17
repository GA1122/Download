double ConvertToBlinkTime(const base::TimeTicks& time_ticks) {
  return (time_ticks - base::TimeTicks()).InSecondsF();
}
