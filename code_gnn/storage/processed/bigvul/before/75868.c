vrrp_garp_refresh_rep_handler(vector_t *strvec)
{
        unsigned repeats;

         
        if (!read_unsigned_strvec(strvec, 1, &repeats, 0, UINT_MAX, true)) {
                report_config_error(CONFIG_GENERAL_ERROR, "vrrp_garp_master_refresh_repeat '%s' invalid - ignoring", FMT_STR_VSLOT(strvec, 1));
                return;
        }

        if (repeats == 0) {
                report_config_error(CONFIG_GENERAL_ERROR, "vrrp_garp_master_refresh_repeat must be greater than 0, setting to 1");
                repeats = 1;
        }

	global_data->vrrp_garp_refresh_rep = repeats;

}
