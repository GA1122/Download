static void set_texcoords_in_vertices(const float coord[4],
                                      float *out, unsigned stride)
{
   out[0] = coord[0];  
   out[1] = coord[1];  
   out += stride;
   out[0] = coord[2];  
   out[1] = coord[1];  
   out += stride;
   out[0] = coord[2];  
   out[1] = coord[3];  
   out += stride;
   out[0] = coord[0];  
   out[1] = coord[3];  
}
