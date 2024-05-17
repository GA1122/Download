MagickExport MagickBooleanType DrawAffineImage(Image *image,
  const Image *source,const AffineMatrix *affine)
{
  AffineMatrix
    inverse_affine;

  CacheView
    *image_view,
    *source_view;

  ExceptionInfo
    *exception;

  MagickBooleanType
    status;

  MagickPixelPacket
    zero;

  PointInfo
    extent[4],
    min,
    max,
    point;

  register ssize_t
    i;

  SegmentInfo
    edge;

  ssize_t
    start,
    stop,
    y;

   
  assert(image != (Image *) NULL);
  assert(image->signature == MagickSignature);
  if (image->debug != MagickFalse)
    (void) LogMagickEvent(TraceEvent,GetMagickModule(),"%s",image->filename);
  assert(source != (const Image *) NULL);
  assert(source->signature == MagickSignature);
  assert(affine != (AffineMatrix *) NULL);
  extent[0].x=0.0;
  extent[0].y=0.0;
  extent[1].x=(double) source->columns-1.0;
  extent[1].y=0.0;
  extent[2].x=(double) source->columns-1.0;
  extent[2].y=(double) source->rows-1.0;
  extent[3].x=0.0;
  extent[3].y=(double) source->rows-1.0;
  for (i=0; i < 4; i++)
  {
    point=extent[i];
    extent[i].x=point.x*affine->sx+point.y*affine->ry+affine->tx;
    extent[i].y=point.x*affine->rx+point.y*affine->sy+affine->ty;
  }
  min=extent[0];
  max=extent[0];
  for (i=1; i < 4; i++)
  {
    if (min.x > extent[i].x)
      min.x=extent[i].x;
    if (min.y > extent[i].y)
      min.y=extent[i].y;
    if (max.x < extent[i].x)
      max.x=extent[i].x;
    if (max.y < extent[i].y)
      max.y=extent[i].y;
  }
   
  if (SetImageStorageClass(image,DirectClass) == MagickFalse)
    return(MagickFalse);
  status=MagickTrue;
  edge.x1=MagickMax(min.x,0.0);
  edge.y1=MagickMax(min.y,0.0);
  edge.x2=MagickMin(max.x,(double) image->columns-1.0);
  edge.y2=MagickMin(max.y,(double) image->rows-1.0);
  inverse_affine=InverseAffineMatrix(affine);
  GetMagickPixelPacket(image,&zero);
  exception=(&image->exception);
  start=(ssize_t) ceil(edge.y1-0.5);
  stop=(ssize_t) floor(edge.y2+0.5);
  source_view=AcquireVirtualCacheView(source,exception);
  image_view=AcquireAuthenticCacheView(image,exception);
#if defined(MAGICKCORE_OPENMP_SUPPORT)
  #pragma omp parallel for schedule(static,4) shared(status) \
    magick_threads(source,image,1,1)
#endif
  for (y=start; y <= stop; y++)
  {
    MagickPixelPacket
      composite,
      pixel;

    PointInfo
      point;

    register IndexPacket
      *magick_restrict indexes;

    register ssize_t
      x;

    register PixelPacket
      *magick_restrict q;

    SegmentInfo
      inverse_edge;

    ssize_t
      x_offset;

    inverse_edge=AffineEdge(source,&inverse_affine,(double) y,&edge);
    if (inverse_edge.x2 < inverse_edge.x1)
      continue;
    q=GetCacheViewAuthenticPixels(image_view,(ssize_t) ceil(inverse_edge.x1-
      0.5),y,(size_t) (floor(inverse_edge.x2+0.5)-ceil(inverse_edge.x1-0.5)+1),
      1,exception);
    if (q == (PixelPacket *) NULL)
      continue;
    indexes=GetCacheViewAuthenticIndexQueue(image_view);
    pixel=zero;
    composite=zero;
    x_offset=0;
    for (x=(ssize_t) ceil(inverse_edge.x1-0.5); x <= (ssize_t) floor(inverse_edge.x2+0.5); x++)
    {
      point.x=(double) x*inverse_affine.sx+y*inverse_affine.ry+
        inverse_affine.tx;
      point.y=(double) x*inverse_affine.rx+y*inverse_affine.sy+
        inverse_affine.ty;
      (void) InterpolateMagickPixelPacket(source,source_view,
        UndefinedInterpolatePixel,point.x,point.y,&pixel,exception);
      SetMagickPixelPacket(image,q,indexes+x_offset,&composite);
      MagickPixelCompositeOver(&pixel,pixel.opacity,&composite,
        composite.opacity,&composite);
      SetPixelPacket(image,&composite,q,indexes+x_offset);
      x_offset++;
      q++;
    }
    if (SyncCacheViewAuthenticPixels(image_view,exception) == MagickFalse)
      status=MagickFalse;
  }
  source_view=DestroyCacheView(source_view);
  image_view=DestroyCacheView(image_view);
  return(status);
}