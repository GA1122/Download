void CL_SetRecommended_f( void ) {
	if ( Cmd_Argc() > 1 ) {
		Com_SetRecommended( qtrue );
	} else {
		Com_SetRecommended( qfalse );
	}

}
