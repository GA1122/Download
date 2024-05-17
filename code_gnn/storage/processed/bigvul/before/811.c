poppler_page_transition_new (void)
{
  return (PopplerPageTransition *) g_new0 (PopplerPageTransition, 1);
}
