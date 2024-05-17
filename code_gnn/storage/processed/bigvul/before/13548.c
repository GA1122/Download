static void  Free_PosRuleSet( HB_PosRuleSet*  prs )
{
  HB_UShort     n, count;

  HB_PosRule*  pr;


  if ( prs->PosRule )
  {
    count = prs->PosRuleCount;
    pr    = prs->PosRule;

    for ( n = 0; n < count; n++ )
      Free_PosRule( &pr[n] );

    FREE( pr );
  }
}
