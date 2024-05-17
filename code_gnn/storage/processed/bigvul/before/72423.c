static void param_dtor(void *data)  
{
	struct pdo_bound_param_data *param = (struct pdo_bound_param_data *)data;
	TSRMLS_FETCH();

	 
	if (param->stmt->methods->param_hook) {
		param->stmt->methods->param_hook(param->stmt, param, PDO_PARAM_EVT_FREE TSRMLS_CC);
	}

	if (param->name) {
		efree(param->name);
	}

	if (param->parameter) {
		zval_ptr_dtor(&(param->parameter));
		param->parameter = NULL;
	}
	if (param->driver_params) {
		zval_ptr_dtor(&(param->driver_params));
	}
}
 