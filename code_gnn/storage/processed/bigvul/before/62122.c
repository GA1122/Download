LosslessReduceDepthOK(Image *image)
{
     

#define QuantumToCharToQuantumEqQuantum(quantum) \
  ((ScaleCharToQuantum((unsigned char) ScaleQuantumToChar(quantum))) == quantum)

    MagickBooleanType
      ok_to_reduce=MagickFalse;

    if (image->depth >= 16)
      {

        const PixelPacket
          *p;

        ok_to_reduce=
           QuantumToCharToQuantumEqQuantum(image->background_color.red) &&
           QuantumToCharToQuantumEqQuantum(image->background_color.green) &&
           QuantumToCharToQuantumEqQuantum(image->background_color.blue) ?
           MagickTrue : MagickFalse;

        if (ok_to_reduce != MagickFalse && image->storage_class == PseudoClass)
          {
            int indx;

            for (indx=0; indx < (ssize_t) image->colors; indx++)
              {
                ok_to_reduce=(
                   QuantumToCharToQuantumEqQuantum(
                   image->colormap[indx].red) &&
                   QuantumToCharToQuantumEqQuantum(
                   image->colormap[indx].green) &&
                   QuantumToCharToQuantumEqQuantum(
                   image->colormap[indx].blue)) ?
                   MagickTrue : MagickFalse;

                if (ok_to_reduce == MagickFalse)
                   break;
              }
          }

        if ((ok_to_reduce != MagickFalse) &&
            (image->storage_class != PseudoClass))
          {
            ssize_t
              y;

            register ssize_t
              x;

            for (y=0; y < (ssize_t) image->rows; y++)
            {
              p=GetVirtualPixels(image,0,y,image->columns,1,&image->exception);

              if (p == (const PixelPacket *) NULL)
                {
                  ok_to_reduce = MagickFalse;
                  break;
                }

              for (x=(ssize_t) image->columns-1; x >= 0; x--)
              {
                ok_to_reduce=
                   QuantumToCharToQuantumEqQuantum(GetPixelRed(p)) &&
                   QuantumToCharToQuantumEqQuantum(GetPixelGreen(p)) &&
                   QuantumToCharToQuantumEqQuantum(GetPixelBlue(p)) ?
                   MagickTrue : MagickFalse;

                if (ok_to_reduce == MagickFalse)
                  break;

                p++;
              }
              if (x >= 0)
                break;
            }
          }

        if (ok_to_reduce != MagickFalse)
          {
            (void) LogMagickEvent(CoderEvent,GetMagickModule(),
                "    OK to reduce PNG bit depth to 8 without loss of info");
          }
        else
          {
            (void) LogMagickEvent(CoderEvent,GetMagickModule(),
                "    Not OK to reduce PNG bit depth to 8 without loss of info");
          }
      }

    return ok_to_reduce;
}
