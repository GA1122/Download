gpc_Gpre(Pixel *out, const Pixel *in, const Background *back)
{
 (void)back;

 if (in->r == in->g && in->g == in->b)
      out->r = out->g = out->b = ilineara_g22(in->g, in->a);

 else
      out->r = out->g = out->b = u16d(in->a * 257 *
 YfromRGB(g22_to_d[in->r], g22_to_d[in->g], g22_to_d[in->b]));

   out->a = 257 * in->a;
}
