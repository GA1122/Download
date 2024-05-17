static void FixSignedValues(PixelPacket *q, int y)
{
  while(y-->0)
  {
      
    SetPixelRed(q,GetPixelRed(q)+QuantumRange/2+1);
    SetPixelGreen(q,GetPixelGreen(q)+QuantumRange/2+1);
    SetPixelBlue(q,GetPixelBlue(q)+QuantumRange/2+1);
    q++;
  }
}
