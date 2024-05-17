	~RuntimeConfigItem() { if (admin) free(admin); if (config) free(config); }
