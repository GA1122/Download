vrrp_vscript_script_handler(__attribute__((unused)) vector_t *strvec)
{
	vrrp_script_t *vscript = LIST_TAIL_DATA(vrrp_data->vrrp_script);
	vector_t *strvec_qe;

	 
	strvec_qe = alloc_strvec_quoted_escaped(NULL);

	set_script_params_array(strvec_qe, &vscript->script, 0);
	free_strvec(strvec_qe);
}