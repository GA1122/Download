static MagickBooleanType ComparePixels(const ImageLayerMethod method,
  const MagickPixelPacket *p,const MagickPixelPacket *q)
{
  MagickRealType
    o1,
    o2;

   
  if (method == CompareAnyLayer)
    return((MagickBooleanType)(IsMagickColorSimilar(p,q) == MagickFalse));

  o1 = (p->matte != MagickFalse) ? GetPixelOpacity(p) : OpaqueOpacity;
  o2 = (q->matte != MagickFalse) ? q->opacity : OpaqueOpacity;

   
  if (method == CompareClearLayer)
    return((MagickBooleanType) ( (o1 <= ((MagickRealType) QuantumRange/2.0)) &&
      (o2 > ((MagickRealType) QuantumRange/2.0)) ) );

   
  if (method == CompareOverlayLayer)
    {
      if (o2 > ((MagickRealType) QuantumRange/2.0))
        return MagickFalse;
      return((MagickBooleanType) (IsMagickColorSimilar(p,q) == MagickFalse));
    }
  return(MagickFalse);
}
