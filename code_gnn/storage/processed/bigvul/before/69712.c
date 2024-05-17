get_max_sample_size(guard_selection_t *gs,
                    int n_guards)
{
  const int using_bridges = (gs->type == GS_TYPE_BRIDGE);
  const int min_sample = get_min_filtered_sample_size();

   
  if (using_bridges)
    return INT_MAX;

  const int max_sample_by_pct = (int)(n_guards * get_max_sample_threshold());
  const int max_sample_absolute = get_max_sample_size_absolute();
  const int max_sample = MIN(max_sample_by_pct, max_sample_absolute);
  if (max_sample < min_sample)
    return min_sample;
  else
    return max_sample;
}
