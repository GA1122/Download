CStarter::publishPreScriptUpdateAd( ClassAd* ad )
{
	if( pre_script && pre_script->PublishUpdateAd(ad) ) {
		return true;
	}
	return false;
}
