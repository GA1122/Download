AviaryScheddPlugin::shutdown()
{
	static bool skip = false;
	if (skip) return; skip = true;

	dprintf(D_FULLDEBUG, "AviaryScheddPlugin: shutting down...\n");

	if (schedulerObj) {
		delete schedulerObj;
		schedulerObj = NULL;
	}
	if (provider) {
		provider->invalidate();
		delete provider;
		provider = NULL;
	}
}
