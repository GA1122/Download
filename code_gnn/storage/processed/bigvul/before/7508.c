  tt_check_trickyness( FT_Face  face )
  {
    if ( !face )
      return FALSE;

     
    if ( face->family_name                               &&
         tt_check_trickyness_family( face->family_name ) )
      return TRUE;

     
     
     
    if ( tt_check_trickyness_sfnt_ids( (TT_Face)face ) )
      return TRUE;

    return FALSE;
  }
