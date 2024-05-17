static void  Free_ChainPosRule( HB_ChainPosRule*  cpr )
{
  FREE( cpr->PosLookupRecord );
  FREE( cpr->Lookahead );
  FREE( cpr->Input );
  FREE( cpr->Backtrack );
}
