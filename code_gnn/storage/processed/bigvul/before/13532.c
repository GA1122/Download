static void  Free_ContextPos3( HB_ContextPosFormat3*  cpf3 )
{
  HB_UShort      n, count;

  HB_Coverage*  c;


  FREE( cpf3->PosLookupRecord );

  if ( cpf3->Coverage )
  {
    count = cpf3->GlyphCount;
    c     = cpf3->Coverage;

    for ( n = 0; n < count; n++ )
      _HB_OPEN_Free_Coverage( &c[n] );

    FREE( c );
  }
}
