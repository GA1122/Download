static unsigned checkColorValidity(LodePNGColorType colortype, unsigned bd)  
{
  switch(colortype)
  {
    case 0: if(!(bd == 1 || bd == 2 || bd == 4 || bd == 8 || bd == 16)) return 37; break;  
    case 2: if(!(                                 bd == 8 || bd == 16)) return 37; break;  
    case 3: if(!(bd == 1 || bd == 2 || bd == 4 || bd == 8            )) return 37; break;  
    case 4: if(!(                                 bd == 8 || bd == 16)) return 37; break;  
    case 6: if(!(                                 bd == 8 || bd == 16)) return 37; break;  
    default: return 31;
  }
  return 0;  
}
