bool ShouldForgetOpenersForTransition(content::PageTransition transition) {
  return transition == content::PAGE_TRANSITION_TYPED ||
      transition == content::PAGE_TRANSITION_AUTO_BOOKMARK ||
      transition == content::PAGE_TRANSITION_GENERATED ||
      transition == content::PAGE_TRANSITION_KEYWORD ||
      transition == content::PAGE_TRANSITION_AUTO_TOPLEVEL;
}
