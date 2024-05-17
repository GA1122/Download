 void handle_pxe_menu(cmd_tbl_t *cmdtp, struct pxe_menu *cfg)
 {
 	void *choice;
 	struct menu *m;
 	int err;
 
 #ifdef CONFIG_CMD_BMP
  	 
  	if (cfg->bmp) {
  		if (get_relfile(cmdtp, cfg->bmp, image_load_addr)) {
			run_command("cls", 0);
// 			if (CONFIG_IS_ENABLED(CMD_CLS))
// 				run_command("cls", 0);
  			bmp_display(image_load_addr,
  				    BMP_ALIGN_CENTER, BMP_ALIGN_CENTER);
  		} else {
 			printf("Skipping background bmp %s for failure\n",
 			       cfg->bmp);
 		}
 	}
 #endif
 
 	m = pxe_menu_to_menu(cfg);
 	if (!m)
 		return;
 
 	err = menu_get_choice(m, &choice);
 
 	menu_destroy(m);
 
 	 
 
 	if (err == 1) {
 		err = label_boot(cmdtp, choice);
 		if (!err)
 			return;
 	} else if (err != -ENOENT) {
 		return;
 	}
 
 	boot_unattempted_labels(cmdtp, cfg);
 }