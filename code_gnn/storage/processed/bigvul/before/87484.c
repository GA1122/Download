static unsigned getNumColorChannels(LodePNGColorType colortype)
{
  switch(colortype)
  {
    case 0: return 1;  
    case 2: return 3;  
    case 3: return 1;  
    case 4: return 2;  
    case 6: return 4;  
  }
  return 0;  
}
