gcm_check_aadlen_or_ivlen (u32 ctr[2])
{
   
  if (ctr[1] > 0x1fffffffU)
    return 0;
  if (ctr[1] < 0x1fffffffU)
    return 1;

  if (ctr[0] <= 0xffffffffU)
    return 1;

  return 0;
}