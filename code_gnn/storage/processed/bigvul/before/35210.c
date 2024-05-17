int __init iriap_init(void)
{
	struct ias_object *obj;
	struct iriap_cb *server;
	__u8 oct_seq[6];
	__u16 hints;

	 
	iriap = hashbin_new(HB_LOCK);
	if (!iriap)
		return -ENOMEM;

	 
	irias_objects = hashbin_new(HB_LOCK);
	if (!irias_objects) {
		IRDA_WARNING("%s: Can't allocate irias_objects hashbin!\n",
			     __func__);
		hashbin_delete(iriap, NULL);
		return -ENOMEM;
	}

	 
	hints  = irlmp_service_to_hint(S_COMPUTER);
	service_handle = irlmp_register_service(hints);

	 
	obj = irias_new_object("Device", IAS_DEVICE_ID);
	irias_add_string_attrib(obj, "DeviceName", "Linux", IAS_KERNEL_ATTR);

	oct_seq[0] = 0x01;   
	oct_seq[1] = 0x00;   
	oct_seq[2] = 0x00;   
#ifdef CONFIG_IRDA_ULTRA
	oct_seq[2] |= 0x04;  
#endif
	irias_add_octseq_attrib(obj, "IrLMPSupport", oct_seq, 3,
				IAS_KERNEL_ATTR);
	irias_insert_object(obj);

	 
	server = iriap_open(LSAP_IAS, IAS_SERVER, NULL, NULL);
	if (!server) {
		IRDA_DEBUG(0, "%s(), unable to open server\n", __func__);
		return -1;
	}
	iriap_register_lsap(server, LSAP_IAS, IAS_SERVER);

	return 0;
}
