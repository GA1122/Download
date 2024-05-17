_HB_GPOS_Free_SubTable( HB_GPOS_SubTable* st,
			HB_UShort         lookup_type )
{
  switch ( lookup_type ) {
    case HB_GPOS_LOOKUP_SINGLE:		Free_SinglePos		( st ); return;
    case HB_GPOS_LOOKUP_PAIR:		Free_PairPos		( st ); return;
    case HB_GPOS_LOOKUP_CURSIVE:	Free_CursivePos		( st ); return;
    case HB_GPOS_LOOKUP_MARKBASE:	Free_MarkBasePos	( st ); return;
    case HB_GPOS_LOOKUP_MARKLIG:	Free_MarkLigPos		( st ); return;
    case HB_GPOS_LOOKUP_MARKMARK:	Free_MarkMarkPos	( st ); return;
    case HB_GPOS_LOOKUP_CONTEXT:	Free_ContextPos		( st ); return;
    case HB_GPOS_LOOKUP_CHAIN:		Free_ChainContextPos	( st ); return;
   
    default:									return;
  }
}
