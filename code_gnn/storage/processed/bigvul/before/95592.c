void Com_ExecuteCfg(void)
{
#ifndef UPDATE_SERVER
	Cbuf_ExecuteText(EXEC_NOW, "exec default.cfg\n");
	if ( FS_ReadFile( "language.cfg", NULL ) > 0 ) {
		Cbuf_ExecuteText(EXEC_APPEND, "exec language.cfg\n");
	} else if ( FS_ReadFile( "Language.cfg", NULL ) > 0 ) {
		Cbuf_ExecuteText(EXEC_APPEND, "exec Language.cfg\n");
	}
	Cbuf_Execute();  

	if(!Com_SafeMode())
	{
		Cbuf_ExecuteText(EXEC_NOW, "exec " Q3CONFIG_CFG "\n");
		Cbuf_Execute();
		Cbuf_ExecuteText(EXEC_NOW, "exec autoexec.cfg\n");
		Cbuf_Execute();
	}
#endif
}
