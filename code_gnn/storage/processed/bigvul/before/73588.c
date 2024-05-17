static MagickBooleanType ReadOneLayer(const ImageInfo *image_info,Image* image,
  XCFDocInfo* inDocInfo,XCFLayerInfo *outLayer,const ssize_t layer,
  ExceptionInfo *exception)
{
  MagickOffsetType
    offset;

  unsigned int
    foundPropEnd = 0;

  size_t
    hierarchy_offset,
    layer_mask_offset;

   
  (void) ResetMagickMemory( outLayer, 0, sizeof( XCFLayerInfo ) );
   
  outLayer->width = ReadBlobMSBLong(image);
  outLayer->height = ReadBlobMSBLong(image);
  outLayer->type = ReadBlobMSBLong(image);
  (void) ReadBlobStringWithLongSize(image, outLayer->name,
    sizeof(outLayer->name),exception);
   
  foundPropEnd = 0;
  while ( (foundPropEnd == MagickFalse) && (EOFBlob(image) == MagickFalse) ) {
  PropType    prop_type = (PropType) ReadBlobMSBLong(image);
  size_t  prop_size = ReadBlobMSBLong(image);
    switch (prop_type)
    {
    case PROP_END:
      foundPropEnd = 1;
      break;
    case PROP_ACTIVE_LAYER:
      outLayer->active = 1;
      break;
    case PROP_FLOATING_SELECTION:
      outLayer->floating_offset = ReadBlobMSBLong(image);
      break;
    case PROP_OPACITY:
      outLayer->alpha = ReadBlobMSBLong(image);
      break;
    case PROP_VISIBLE:
      outLayer->visible = ReadBlobMSBLong(image);
      break;
    case PROP_LINKED:
      outLayer->linked = ReadBlobMSBLong(image);
      break;
    case PROP_PRESERVE_TRANSPARENCY:
      outLayer->preserve_trans = ReadBlobMSBLong(image);
      break;
    case PROP_APPLY_MASK:
      outLayer->apply_mask = ReadBlobMSBLong(image);
      break;
    case PROP_EDIT_MASK:
      outLayer->edit_mask = ReadBlobMSBLong(image);
      break;
    case PROP_SHOW_MASK:
      outLayer->show_mask = ReadBlobMSBLong(image);
      break;
    case PROP_OFFSETS:
      outLayer->offset_x = (int) ReadBlobMSBLong(image);
      outLayer->offset_y = (int) ReadBlobMSBLong(image);
      break;
    case PROP_MODE:
      outLayer->mode = ReadBlobMSBLong(image);
      break;
    case PROP_TATTOO:
      outLayer->preserve_trans = ReadBlobMSBLong(image);
      break;
     case PROP_PARASITES:
     {
       if (DiscardBlobBytes(image,prop_size) == MagickFalse)
         ThrowFileException(exception,CorruptImageError,
           "UnexpectedEndOfFile",image->filename);

         
     }
     break;
    default:
       

      {
      int buf[16];
      ssize_t amount;

       
      while ((prop_size > 0) && (EOFBlob(image) == MagickFalse))
        {
        amount = (ssize_t) MagickMin(16, prop_size);
        amount = ReadBlob(image, (size_t) amount, (unsigned char *) &buf);
        if (!amount)
          ThrowBinaryException(CorruptImageError,"CorruptImage",
            image->filename);
        prop_size -= (size_t) MagickMin(16, (size_t) amount);
        }
      }
      break;
    }
  }

  if (foundPropEnd == MagickFalse)
    return(MagickFalse);
   
  if (image_info->number_scenes != 0)
    {
      ssize_t
        scene;

      scene=inDocInfo->number_layers-layer-1;
      if (scene > (ssize_t) (image_info->scene+image_info->number_scenes-1))
        {
          outLayer->image=CloneImage(image,0,0,MagickTrue,exception);
          if (outLayer->image == (Image *) NULL)
            return(MagickFalse);
          InitXCFImage(outLayer,exception);
          return(MagickTrue);
        }
    }
  outLayer->image=CloneImage(image,outLayer->width, outLayer->height,MagickTrue,
    exception);
  if (outLayer->image == (Image *) NULL)
    return(MagickFalse);
   
  outLayer->image->background_color.alpha=
    ScaleCharToQuantum((unsigned char) outLayer->alpha);
  (void) SetImageBackgroundColor(outLayer->image,exception);

  InitXCFImage(outLayer,exception);

   
  outLayer->image->compose = GIMPBlendModeToCompositeOperator( outLayer->mode );
  if ( outLayer->visible == MagickFalse )
    {
       
      outLayer->image->compose = NoCompositeOp;
    }

   
  hierarchy_offset = ReadBlobMSBLong(image);
  layer_mask_offset = ReadBlobMSBLong(image);

   
  offset=SeekBlob(image, (MagickOffsetType) hierarchy_offset, SEEK_SET);
  if (offset < 0)
    (void) ThrowMagickException(exception,GetMagickModule(),
      CorruptImageError,"InvalidImageHeader","`%s'",image->filename);
  if (load_hierarchy (image, inDocInfo, outLayer, exception) == 0)
    return(MagickFalse);

   
  if (layer_mask_offset != 0)
    {
      offset=SeekBlob(image, (MagickOffsetType) layer_mask_offset, SEEK_SET);

#if 0   
      layer_mask = xcf_load_layer_mask (info, gimage);
      if (layer_mask == 0)
  goto error;

       
      GIMP_DRAWABLE (layer_mask)->offset_x = GIMP_DRAWABLE (layer)->offset_x;
      GIMP_DRAWABLE (layer_mask)->offset_y = GIMP_DRAWABLE (layer)->offset_y;

      gimp_layer_add_mask (layer, layer_mask, MagickFalse);

      layer->mask->apply_mask = apply_mask;
      layer->mask->edit_mask  = edit_mask;
      layer->mask->show_mask  = show_mask;
#endif
  }

   
#if 0   
  if (add_floating_sel)
    {
      GimpLayer *floating_sel;

      floating_sel = info->floating_sel;
      floating_sel_attach (floating_sel, GIMP_DRAWABLE (layer));
    }
#endif

  return MagickTrue;
}
