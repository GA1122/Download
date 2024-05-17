static void  Free_LigatureArray( HB_LigatureArray*  la,
				 HB_UShort           num_classes )
{
  HB_UShort            n, count;

  HB_LigatureAttach*  lat;


  if ( la->LigatureAttach )
  {
    count = la->LigatureCount;
    lat   = la->LigatureAttach;

    for ( n = 0; n < count; n++ )
      Free_LigatureAttach( &lat[n], num_classes );

    FREE( lat );
  }
}
