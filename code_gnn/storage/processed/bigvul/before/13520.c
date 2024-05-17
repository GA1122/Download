static void  Free_Anchor( HB_Anchor*  an)
{
  if ( an->PosFormat == 3 && an->af.af3.DeviceTables )
  {
    _HB_OPEN_Free_Device( an->af.af3.DeviceTables[AF3_X_DEVICE_TABLE] );
    _HB_OPEN_Free_Device( an->af.af3.DeviceTables[AF3_Y_DEVICE_TABLE] );
    FREE( an->af.af3.DeviceTables );
  }
}
