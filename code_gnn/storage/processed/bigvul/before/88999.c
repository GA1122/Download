static void GenerateCLAHELut(const RangeInfo *range_info,
  const size_t number_bins,unsigned short *lut)
{
  ssize_t
    i;

  unsigned short
    delta;

   
  delta=(unsigned short) ((range_info->max-range_info->min)/number_bins+1);
  for (i=(ssize_t) range_info->min; i <= (ssize_t) range_info->max; i++)
    lut[i]=(unsigned short) ((i-range_info->min)/delta);
}
