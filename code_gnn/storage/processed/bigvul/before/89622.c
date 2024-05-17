gcm_check_datalen (u32 ctr[2])
{
   
  if (ctr[1] > 0xfU)
    return 0;
  if (ctr[1] < 0xfU)
    return 1;

  if (ctr[0] <= 0xffffffe0U)
    return 1;

  return 0;
}
