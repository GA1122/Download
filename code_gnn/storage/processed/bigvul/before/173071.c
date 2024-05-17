gpc_Pre(Pixel *out, const Pixel *in, const Background *back)
{
 (void)back;

   out->r = ilineara_g22(in->r, in->a);

 if (in->g == in->r)
 {
      out->g = out->r;

 if (in->b == in->r)
         out->b = out->r;

 else
         out->b = ilineara_g22(in->b, in->a);
 }

 else
 {
      out->g = ilineara_g22(in->g, in->a);

 if (in->b == in->r)
         out->b = out->r;

 else if (in->b == in->g)
         out->b = out->g;

 else
         out->b = ilineara_g22(in->b, in->a);
 }

   out->a = in->a * 257;
}
