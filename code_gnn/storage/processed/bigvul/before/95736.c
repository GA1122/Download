void CL_UI_Restart_f( void ) {           
	CL_ShutdownUI();

	autoupdateChecked = qfalse;

	CL_InitUI();
}
