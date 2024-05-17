HB_Error  HB_GPOS_Query_Features( HB_GPOSHeader*  gpos,
				  HB_UShort        script_index,
				  HB_UShort        language_index,
				  HB_UInt**       feature_tag_list )
{
  HB_UShort           n;
  HB_Error            error;
  HB_UInt*           ftl;

  HB_ScriptList*     sl;
  HB_ScriptRecord*   sr;
  HB_ScriptTable*    s;
  HB_LangSysRecord*  lsr;
  HB_LangSys*        ls;
  HB_UShort*          fi;

  HB_FeatureList*    fl;
  HB_FeatureRecord*  fr;


  if ( !gpos || !feature_tag_list )
    return ERR(HB_Err_Invalid_Argument);

  sl = &gpos->ScriptList;
  sr = sl->ScriptRecord;

  fl = &gpos->FeatureList;
  fr = fl->FeatureRecord;

  if ( script_index >= sl->ScriptCount )
    return ERR(HB_Err_Invalid_Argument);

  s   = &sr[script_index].Script;
  lsr = s->LangSysRecord;

  if ( language_index == 0xFFFF )
    ls = &s->DefaultLangSys;
  else
  {
    if ( language_index >= s->LangSysCount )
      return ERR(HB_Err_Invalid_Argument);

    ls = &lsr[language_index].LangSys;
  }

  fi = ls->FeatureIndex;

  if ( ALLOC_ARRAY( ftl, ls->FeatureCount + 1, HB_UInt ) )
    return error;

  for ( n = 0; n < ls->FeatureCount; n++ )
  {
    if ( fi[n] >= fl->FeatureCount )
    {
      FREE( ftl );
      return ERR(HB_Err_Invalid_SubTable_Format);
    }
    ftl[n] = fr[fi[n]].FeatureTag;
  }
  ftl[n] = 0;

  *feature_tag_list = ftl;

  return HB_Err_Ok;
}
