bool ShouldNavigateForward(const NavigationController& controller,
                           OverscrollMode mode) {
  return mode == (base::i18n::IsRTL() ? OVERSCROLL_EAST : OVERSCROLL_WEST) &&
         controller.CanGoForward();
}
