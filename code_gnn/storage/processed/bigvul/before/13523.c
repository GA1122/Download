static void  Free_ChainContextPos2( HB_ChainContextPosFormat2*  ccpf2 )
{
  HB_UShort              n, count;

  HB_ChainPosClassSet*  cpcs;


  if ( ccpf2->ChainPosClassSet )
  {
    count = ccpf2->ChainPosClassSetCount;
    cpcs  = ccpf2->ChainPosClassSet;

    for ( n = 0; n < count; n++ )
      Free_ChainPosClassSet( &cpcs[n] );

    FREE( cpcs );
  }

  _HB_OPEN_Free_ClassDefinition( &ccpf2->LookaheadClassDef );
  _HB_OPEN_Free_ClassDefinition( &ccpf2->InputClassDef );
  _HB_OPEN_Free_ClassDefinition( &ccpf2->BacktrackClassDef );

  _HB_OPEN_Free_Coverage( &ccpf2->Coverage );
}
