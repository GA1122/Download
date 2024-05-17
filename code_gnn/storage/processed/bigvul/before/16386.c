void CStarter::FinalCleanup()
{
	RemoveRecoveryFile();
	removeTempExecuteDir();
}
