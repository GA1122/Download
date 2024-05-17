gpc_Glin(Pixel *out, const Pixel *in, const Background *back)
{
 (void)back;

 if (in->r == in->g && in->g == in->b)
      out->r = out->g = out->b = ilinear_g22(in->g);

 else
      out->r = out->g = out->b = u16d(65535 *
 YfromRGB(g22_to_d[in->r], g22_to_d[in->g], g22_to_d[in->b]));

   out->a = 65535;
}
