static ssize_t ap_control_domain_mask_show(struct bus_type *bus, char *buf)
{
	if (ap_configuration != NULL) {  
		if (test_facility(76)) {  
			return snprintf(buf, PAGE_SIZE,
				"0x%08x%08x%08x%08x%08x%08x%08x%08x\n",
			ap_configuration->adm[0], ap_configuration->adm[1],
			ap_configuration->adm[2], ap_configuration->adm[3],
			ap_configuration->adm[4], ap_configuration->adm[5],
			ap_configuration->adm[6], ap_configuration->adm[7]);
		} else {  
			return snprintf(buf, PAGE_SIZE, "%08x%08x\n",
			ap_configuration->adm[0], ap_configuration->adm[1]);
		  }
	} else {
		return snprintf(buf, PAGE_SIZE, "not supported\n");
	  }
}
