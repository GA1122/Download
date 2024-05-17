void CL_ConsolePrint( char *txt ) {
	int	y, l;
	unsigned char	c;
	unsigned short	color;
	qboolean skipnotify = qfalse;        
	int prev;                            

	if ( !Q_strncmp( txt, "[skipnotify]", 12 ) ) {
		skipnotify = qtrue;
		txt += 12;
	}

	if ( cl_noprint && cl_noprint->integer ) {
		return;
	}

	if ( !con.initialized ) {
		con.color[0] =
			con.color[1] =
				con.color[2] =
					con.color[3] = 1.0f;
		con.linewidth = -1;
		Con_CheckResize();
		con.initialized = qtrue;
	}

	color = ColorIndex( COLNSOLE_COLOR );

	while ( (c = *((unsigned char *) txt)) != 0 ) {
		if ( Q_IsColorString( txt ) ) {
			color = ColorIndex( *( txt + 1 ) );
			txt += 2;
			continue;
		}

		for ( l = 0 ; l < con.linewidth ; l++ ) {
			if ( txt[l] <= ' ' ) {
				break;
			}

		}

		if ( l != con.linewidth && ( con.x + l >= con.linewidth ) ) {
			Con_Linefeed( skipnotify );

		}

		txt++;

		switch ( c )
		{
		case '\n':
			Con_Linefeed( skipnotify );
			break;
		case '\r':
			con.x = 0;
			break;
		default:     
			y = con.current % con.totallines;
			con.text[y * con.linewidth + con.x] = ( color << 8 ) | c;
			con.x++;
			if(con.x >= con.linewidth)
				Con_Linefeed( skipnotify );
			break;
		}
	}

	if ( con.current >= 0 ) {
		if ( skipnotify ) {
			prev = con.current % NUM_CON_TIMES - 1;
			if ( prev < 0 ) {
				prev = NUM_CON_TIMES - 1;
			}
			con.times[prev] = 0;
		} else {
			con.times[con.current % NUM_CON_TIMES] = cls.realtime;
		}
	}
}
