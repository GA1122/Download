void Con_DrawConsole( void ) {
	Con_CheckResize();

	if ( clc.state == CA_DISCONNECTED ) {
		if ( !( Key_GetCatcher( ) & (KEYCATCH_UI | KEYCATCH_CGAME)) ) {
			Con_DrawSolidConsole( 1.0 );
			return;
		}
	}

	if ( con.displayFrac ) {
		Con_DrawSolidConsole( con.displayFrac );
	} else {
		if ( clc.state == CA_ACTIVE ) {
			Con_DrawNotify();
		}
	}
}
