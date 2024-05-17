AviaryScheddPlugin::update(int cmd, const ClassAd *ad)
{
	MyString hashKey;

	switch (cmd) {
	case UPDATE_SCHEDD_AD:
		dprintf(D_FULLDEBUG, "Received UPDATE_SCHEDD_AD\n");
		schedulerObj->update(*ad);
		break;
	default:
		dprintf(D_FULLDEBUG, "Unsupported command: %s\n",
				getCollectorCommandString(cmd));
	}
}
