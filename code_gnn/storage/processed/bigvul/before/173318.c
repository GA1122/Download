skip_chunk_type(const struct global *global, png_uint_32 type)
  
{
  
 if (CRITICAL(type))
 return 0;

 switch (type)
 {
  
 case png_tRNS: case png_sBIT:
 return 0;

  
 case png_gAMA: case png_sRGB:
 if (global->skip >= SKIP_ALL)
 return 1;
 return 0;

  
 case png_cHRM: case png_iCCP:
 if (global->skip >= SKIP_COLOR)
 return 1;
 return 0;

  
 case png_bKGD:
 if (global->skip >= SKIP_TRANSFORM)
 return 1;
 return 0;

  
 case png_fRAc: case png_gIFg: case png_gIFt: case png_gIFx: case png_hIST:
 case png_iTXt: case png_oFFs: case png_pCAL: case png_pHYs: case png_sCAL:
 case png_sPLT: case png_sTER: case png_tEXt: case png_tIME: case png_zTXt:
 if (global->skip >= SKIP_UNUSED)
 return 1;
 return 0;

  
 default:
 if (SAFE_TO_COPY(type))
 {
 if (global->skip >= SKIP_UNUSED)  
 return 1;
 }

 else if (global->skip >= SKIP_UNSAFE)
 return 1;

 return 0;
 }
}
