static void FixSignedValues(const Image *image,Quantum *q, int y)
{
  while(y-->0)
  {
      
    SetPixelRed(image,GetPixelRed(image,q)+QuantumRange/2+1,q);
    SetPixelGreen(image,GetPixelGreen(image,q)+QuantumRange/2+1,q);
    SetPixelBlue(image,GetPixelBlue(image,q)+QuantumRange/2+1,q);
    q++;
  }
}
