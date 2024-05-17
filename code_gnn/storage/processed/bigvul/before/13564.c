HB_Error  HB_GPOS_Select_Language( HB_GPOSHeader*  gpos,
				   HB_UInt         language_tag,
				   HB_UShort        script_index,
				   HB_UShort*       language_index,
				   HB_UShort*       req_feature_index )
{
  HB_UShort           n;

  HB_ScriptList*     sl;
  HB_ScriptRecord*   sr;
  HB_ScriptTable*         s;
  HB_LangSysRecord*  lsr;


  if ( !gpos || !language_index || !req_feature_index )
    return ERR(HB_Err_Invalid_Argument);

  sl = &gpos->ScriptList;
  sr = sl->ScriptRecord;

  if ( script_index >= sl->ScriptCount )
    return ERR(HB_Err_Invalid_Argument);

  s   = &sr[script_index].Script;
  lsr = s->LangSysRecord;

  for ( n = 0; n < s->LangSysCount; n++ )
    if ( language_tag == lsr[n].LangSysTag )
    {
      *language_index = n;
      *req_feature_index = lsr[n].LangSys.ReqFeatureIndex;

      return HB_Err_Ok;
    }

  return HB_Err_Not_Covered;
}
