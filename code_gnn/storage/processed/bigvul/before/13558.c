HB_Error  HB_GPOS_Clear_Features( HB_GPOSHeader*  gpos )
{
  HB_UShort i;

  HB_UInt*  properties;


  if ( !gpos )
    return ERR(HB_Err_Invalid_Argument);

  gpos->FeatureList.ApplyCount = 0;

  properties = gpos->LookupList.Properties;

  for ( i = 0; i < gpos->LookupList.LookupCount; i++ )
    properties[i] = 0;

  return HB_Err_Ok;
}
