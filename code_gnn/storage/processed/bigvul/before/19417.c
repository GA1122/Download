static void efx_remove_all(struct efx_nic *efx)
{
	efx_remove_filters(efx);
	efx_remove_channels(efx);
	efx_remove_port(efx);
	efx_remove_nic(efx);
}
