rend_max_intro_circs_per_period(unsigned int n_intro_points_wanted)
{
   
  tor_assert(n_intro_points_wanted <= NUM_INTRO_POINTS_MAX);
  return (int)(2*n_intro_points_wanted + NUM_INTRO_POINTS_EXTRA);
}
