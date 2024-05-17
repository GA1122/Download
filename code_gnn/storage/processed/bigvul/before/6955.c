  count_faces_sfnt( char*  fond_data )
  {
     
     

    return EndianS16_BtoN( *( (short*)( fond_data +
                                        sizeof ( FamRec ) ) ) ) + 1;
  }
