floorlog10 (double x)
{
  int exp;
  double y;
  double z;
  double l;

   
  y = frexp (x, &exp);
  if (!(y >= 0.0 && y < 1.0))
    abort ();
  if (y == 0.0)
    return INT_MIN;
  if (y < 0.5)
    {
      while (y < (1.0 / (1 << (GMP_LIMB_BITS / 2)) / (1 << (GMP_LIMB_BITS / 2))))
        {
          y *= 1.0 * (1 << (GMP_LIMB_BITS / 2)) * (1 << (GMP_LIMB_BITS / 2));
          exp -= GMP_LIMB_BITS;
        }
      if (y < (1.0 / (1 << 16)))
        {
          y *= 1.0 * (1 << 16);
          exp -= 16;
        }
      if (y < (1.0 / (1 << 8)))
        {
          y *= 1.0 * (1 << 8);
          exp -= 8;
        }
      if (y < (1.0 / (1 << 4)))
        {
          y *= 1.0 * (1 << 4);
          exp -= 4;
        }
      if (y < (1.0 / (1 << 2)))
        {
          y *= 1.0 * (1 << 2);
          exp -= 2;
        }
      if (y < (1.0 / (1 << 1)))
        {
          y *= 1.0 * (1 << 1);
          exp -= 1;
        }
    }
  if (!(y >= 0.5 && y < 1.0))
    abort ();
   
  l = exp;
  z = y;
  if (z < 0.70710678118654752444)
    {
      z *= 1.4142135623730950488;
      l -= 0.5;
    }
  if (z < 0.8408964152537145431)
    {
      z *= 1.1892071150027210667;
      l -= 0.25;
    }
  if (z < 0.91700404320467123175)
    {
      z *= 1.0905077326652576592;
      l -= 0.125;
    }
  if (z < 0.9576032806985736469)
    {
      z *= 1.0442737824274138403;
      l -= 0.0625;
    }
   
  z = 1 - z;
   
  l -= 1.4426950408889634074 * z * (1.0 + z * (0.5 + z * ((1.0 / 3) + z * 0.25)));
   
  l *= 0.30102999566398119523;
   
  return (int) l + (l < 0 ? -1 : 0);
}