proto_reg_handoff_wbxml(void)
{
	dissector_handle_t wbxml_handle;

	 

	wbxml_handle = find_dissector("wbxml");

	 

	 

	dissector_add_string("media_type",
			     "application/vnd.wap.wmlc", wbxml_handle);
	dissector_add_string("media_type",
			     "application/vnd.wap.wta-eventc", wbxml_handle);
	dissector_add_string("media_type",
			     "application/vnd.wap.wbxml", wbxml_handle);
	dissector_add_string("media_type",
			     "application/vnd.wap.sic", wbxml_handle);
	dissector_add_string("media_type",
			     "application/vnd.wap.slc", wbxml_handle);
	dissector_add_string("media_type",
			     "application/vnd.wap.coc", wbxml_handle);
	dissector_add_string("media_type",
			     "application/vnd.wap.connectivity-wbxml", wbxml_handle);
	dissector_add_string("media_type",
			     "application/vnd.wap.locc+wbxml", wbxml_handle);
	dissector_add_string("media_type",
			     "application/vnd.syncml+wbxml", wbxml_handle);
	dissector_add_string("media_type",
			     "application/vnd.syncml.dm+wbxml", wbxml_handle);
	dissector_add_string("media_type",
			     "application/vnd.oma.drm.rights+wbxml", wbxml_handle);
	dissector_add_string("media_type",
			     "application/vnd.wv.csp.wbxml", wbxml_handle);
	dissector_add_string("media_type",
			     "application/vnd.ms-sync.wbxml", wbxml_handle);
	dissector_add_string("media_type",
			     "application/vnd.ms-sync", wbxml_handle);

	 

	dissector_add_string("media_type",
			     "application/vnd.uplanet.cacheop-wbxml", wbxml_handle);
	dissector_add_string("media_type",
			     "application/vnd.uplanet.alert-wbxml", wbxml_handle);
	dissector_add_string("media_type",
			     "application/vnd.uplanet.list-wbxml", wbxml_handle);
	dissector_add_string("media_type",
			     "application/vnd.uplanet.listcmd-wbxml", wbxml_handle);
	dissector_add_string("media_type",
			     "application/vnd.uplanet.channel-wbxml", wbxml_handle);
	dissector_add_string("media_type",
			     "application/vnd.uplanet.bearer-choice-wbxml", wbxml_handle);
	dissector_add_string("media_type",
			     "application/vnd.phonecom.mmc-wbxml", wbxml_handle);
	dissector_add_string("media_type",
			     "application/vnd.nokia.syncset+wbxml", wbxml_handle);

	 
	dissector_add_string("media_type",
			     "application/x-wap-prov.browser-bookmarks", wbxml_handle);
	dissector_add_string("media_type",
			     "application/x-wap-prov.browser-settings", wbxml_handle);
	 
	dissector_add_string("media_type",
			     "application/x-prov.syncset+wbxml", wbxml_handle);
}
