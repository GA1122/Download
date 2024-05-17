static bool LineDashSequenceIsValid(const Vector<double>& dash) {
  for (size_t i = 0; i < dash.size(); i++) {
    if (!std::isfinite(dash[i]) || dash[i] < 0)
      return false;
  }
  return true;
}
