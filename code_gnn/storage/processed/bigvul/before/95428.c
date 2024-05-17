void Con_DrawSolidConsole( float frac ) {
	int				i, x, y;
	int				rows;
	short			*text;
	int				row;
	int				lines;
	int				currentColor;
	vec4_t			color;

	lines = cls.glconfig.vidHeight * frac;
	if (lines <= 0)
		return;

	if (lines > cls.glconfig.vidHeight )
		lines = cls.glconfig.vidHeight;

	con.xadjust = 0;
	SCR_AdjustFrom640( &con.xadjust, NULL, NULL, NULL );

	y = frac * SCREEN_HEIGHT;
	if ( y < 1 ) {
		y = 0;
	}
	else {
		SCR_DrawPic( 0, 0, SCREEN_WIDTH, y, cls.consoleShader );
	}

	color[0] = 1;
	color[1] = 0;
	color[2] = 0;
	color[3] = 1;
	SCR_FillRect( 0, y, SCREEN_WIDTH, 2, color );



	re.SetColor( g_color_table[ColorIndex(COLOR_RED)] );

	i = strlen( Q3_VERSION );

	for (x=0 ; x<i ; x++) {
		SCR_DrawSmallChar( cls.glconfig.vidWidth - ( i - x + 1 ) * SMALLCHAR_WIDTH,
			lines - SMALLCHAR_HEIGHT, Q3_VERSION[x] );
	}


	con.vislines = lines;
	rows = (lines-SMALLCHAR_HEIGHT)/SMALLCHAR_HEIGHT;		 

	y = lines - (SMALLCHAR_HEIGHT*3);

	if (con.display != con.current)
	{
		re.SetColor( g_color_table[ColorIndex(COLOR_RED)] );
		for (x=0 ; x<con.linewidth ; x+=4)
			SCR_DrawSmallChar( con.xadjust + (x+1)*SMALLCHAR_WIDTH, y, '^' );
		y -= SMALLCHAR_HEIGHT;
		rows--;
	}
	
	row = con.display;

	if ( con.x == 0 ) {
		row--;
	}

	currentColor = 7;
	re.SetColor( g_color_table[currentColor] );

	for (i=0 ; i<rows ; i++, y -= SMALLCHAR_HEIGHT, row--)
	{
		if (row < 0)
			break;
		if (con.current - row >= con.totallines) {
			continue;	
		}

		text = con.text + (row % con.totallines)*con.linewidth;

		for (x=0 ; x<con.linewidth ; x++) {
			if ( ( text[x] & 0xff ) == ' ' ) {
				continue;
			}

			if ( ColorIndexForNumber( text[x]>>8 ) != currentColor ) {
				currentColor = ColorIndexForNumber( text[x]>>8 );
				re.SetColor( g_color_table[currentColor] );
			}
			SCR_DrawSmallChar(  con.xadjust + (x+1)*SMALLCHAR_WIDTH, y, text[x] & 0xff );
		}
	}

	Con_DrawInput ();

	re.SetColor( NULL );
}