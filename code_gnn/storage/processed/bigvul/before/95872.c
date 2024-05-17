void CL_ClearState( void ) {

	S_StopAllSounds();

	memset( &cl, 0, sizeof( cl ) );
}
