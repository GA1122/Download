static void Com_Crash_f( void ) {
	* ( volatile int * ) 0 = 0x12345678;
}
