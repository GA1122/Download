static void  Free_MarkLigPos( HB_GPOS_SubTable* st)
{
  HB_MarkLigPos*  mlp = &st->marklig;

  Free_LigatureArray( &mlp->LigatureArray, mlp->ClassCount );
  Free_MarkArray( &mlp->MarkArray );
  _HB_OPEN_Free_Coverage( &mlp->LigatureCoverage );
  _HB_OPEN_Free_Coverage( &mlp->MarkCoverage );
}
