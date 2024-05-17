static int exec_done_cb(zend_module_entry *module TSRMLS_DC)  
{
	if (module->post_deactivate_func) {
		module->post_deactivate_func();
	}
	return 0;
}
 
