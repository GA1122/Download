gpc_Lin(Pixel *out, const Pixel *in, const Background *back)
{
 (void)back;

   out->r = ilinear_g22(in->r);

 if (in->g == in->r)
 {
      out->g = out->r;

 if (in->b == in->r)
         out->b = out->r;

 else
         out->b = ilinear_g22(in->b);
 }

 else
 {
      out->g = ilinear_g22(in->g);

 if (in->b == in->r)
         out->b = out->r;

 else if (in->b == in->g)
         out->b = out->g;

 else
         out->b = ilinear_g22(in->b);
 }

   out->a = 65535;
}
