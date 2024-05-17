static void ipa_rop_draw(wmfAPI * API, wmfROP_Draw_t * rop_draw)
{
 
 

 
 

  if (TO_FILL(rop_draw) == 0)
    return;

   
  (void) PushDrawingWand(WmfDrawingWand);

   

   

 
 

  switch (rop_draw->ROP)  
    {
    case SRCCOPY:  
      printf("ipa_rop_draw SRCCOPY ROP mode not implemented\n");
      break;
    case SRCPAINT:  
      printf("ipa_rop_draw SRCPAINT ROP mode not implemented\n");
      break;
    case SRCAND:  
      printf("ipa_rop_draw SRCAND ROP mode not implemented\n");
      break;
    case SRCINVERT:  
      printf("ipa_rop_draw SRCINVERT ROP mode not implemented\n");
      break;
    case SRCERASE:  
      printf("ipa_rop_draw SRCERASE ROP mode not implemented\n");
      break;
    case NOTSRCCOPY:  
      printf("ipa_rop_draw NOTSRCCOPY ROP mode not implemented\n");
      break;
    case NOTSRCERASE:  
      printf("ipa_rop_draw NOTSRCERASE ROP mode not implemented\n");
      break;
    case MERGECOPY:  
      printf("ipa_rop_draw MERGECOPY ROP mode not implemented\n");
      break;
    case MERGEPAINT:  
      printf("ipa_rop_draw MERGEPAINT ROP mode not implemented\n");
      break;
    case PATCOPY:  
      util_set_brush(API, rop_draw->dc, BrushApplyFill);
      break;
    case PATPAINT:  
      printf("ipa_rop_draw PATPAINT ROP mode not implemented\n");
      break;
    case PATINVERT:  
      printf("ipa_rop_draw PATINVERT ROP mode not implemented\n");
      break;
    case DSTINVERT:  
      printf("ipa_rop_draw DSTINVERT ROP mode not implemented\n");
      break;
    case BLACKNESS:  
      draw_fill_color_string(WmfDrawingWand,"black");
      break;
    case WHITENESS:  
      draw_fill_color_string(WmfDrawingWand,"white");
      break;
    default:
      printf("ipa_rop_draw 0x%x ROP mode not implemented\n", rop_draw->ROP);
      break;
    }

  DrawRectangle(WmfDrawingWand,
                 XC(rop_draw->TL.x), YC(rop_draw->TL.y),
                 XC(rop_draw->BR.x), YC(rop_draw->BR.y));

   
  (void) PopDrawingWand(WmfDrawingWand);
}
