MagickExport void CompositeLayers(Image *destination,
  const CompositeOperator compose, Image *source,const ssize_t x_offset,
  const ssize_t y_offset,ExceptionInfo *exception)
{
  assert(destination != (Image *) NULL);
  assert(destination->signature == MagickCoreSignature);
  assert(source != (Image *) NULL);
  assert(source->signature == MagickCoreSignature);
  assert(exception != (ExceptionInfo *) NULL);
  assert(exception->signature == MagickCoreSignature);
  if (source->debug != MagickFalse || destination->debug != MagickFalse)
    (void) LogMagickEvent(TraceEvent,GetMagickModule(),"%s - %s",
      source->filename,destination->filename);

   
  if ( source->next == (Image *) NULL )
    while ( destination != (Image *) NULL )
    {
      CompositeCanvas(destination,compose,source,x_offset,y_offset);
      destination=GetNextImageInList(destination);
    }

   
  else if ( destination->next == (Image *) NULL )
  {
    Image *dest = CloneImage(destination,0,0,MagickTrue,exception);

    CompositeCanvas(destination,compose,source,x_offset,y_offset);
     
    if ( source->next != (Image *) NULL )
      {
        destination->delay = source->delay;
        destination->iterations = source->iterations;
      }
    source=GetNextImageInList(source);

    while ( source != (Image *) NULL )
    {
      AppendImageToList(&destination,
           CloneImage(dest,0,0,MagickTrue,exception));
      destination=GetLastImageInList(destination);

      CompositeCanvas(destination,compose,source,x_offset,y_offset);
      destination->delay = source->delay;
      destination->iterations = source->iterations;
      source=GetNextImageInList(source);
    }
    dest=DestroyImage(dest);
  }

   
  else
    while ( source != (Image *) NULL && destination != (Image *) NULL )
    {
      CompositeCanvas(destination,compose,source,x_offset,y_offset);
      source=GetNextImageInList(source);
      destination=GetNextImageInList(destination);
    }
}
