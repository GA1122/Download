paeth(uint8x8_t a, uint8x8_t b, uint8x8_t c)
{
 uint8x8_t d, e;
 uint16x8_t p1, pa, pb, pc;

   p1 = vaddl_u8(a, b);  
   pc = vaddl_u8(c, c);  
   pa = vabdl_u8(b, c);  
   pb = vabdl_u8(a, c);  
   pc = vabdq_u16(p1, pc);  

   p1 = vcleq_u16(pa, pb);  
   pa = vcleq_u16(pa, pc);  
   pb = vcleq_u16(pb, pc);  

   p1 = vandq_u16(p1, pa);  

   d = vmovn_u16(pb);
   e = vmovn_u16(p1);

   d = vbsl_u8(d, b, c);
   e = vbsl_u8(e, a, d);

 return e;
}
