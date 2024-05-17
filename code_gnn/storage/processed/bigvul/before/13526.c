static void  Free_ChainPosClassSet( HB_ChainPosClassSet*  cpcs )
{
  HB_UShort               n, count;

  HB_ChainPosClassRule*  cpcr;


  if ( cpcs->ChainPosClassRule )
  {
    count = cpcs->ChainPosClassRuleCount;
    cpcr  = cpcs->ChainPosClassRule;

    for ( n = 0; n < count; n++ )
      Free_ChainPosClassRule( &cpcr[n] );

    FREE( cpcr );
  }
}
