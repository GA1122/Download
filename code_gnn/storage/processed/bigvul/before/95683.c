void CL_InitRenderer( void ) {
	re.BeginRegistration( &cls.glconfig );

	cls.charSetShader = re.RegisterShader( "gfx/2d/hudchars" );
	cls.whiteShader = re.RegisterShader( "white" );
	cls.consoleShader = re.RegisterShader( "console-16bit" );  
	cls.consoleShader2 = re.RegisterShader( "console2-16bit" );  
	g_console_field_width = cls.glconfig.vidWidth / SMALLCHAR_WIDTH - 2;
	g_consoleField.widthInChars = g_console_field_width;
}
