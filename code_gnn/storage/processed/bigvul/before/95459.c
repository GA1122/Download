void Com_GameRestart_f(void)
{
	if(!FS_FilenameCompare(Cmd_Argv(1), com_basegame->string))
	{
		
		Cvar_Set("fs_game", "");
	}
	else
		Cvar_Set("fs_game", Cmd_Argv(1));

	Com_GameRestart(0, qtrue);
}
