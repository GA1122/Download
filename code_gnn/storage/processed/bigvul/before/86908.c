static TEE_Result get_prop_tee_dev_id(struct tee_ta_session *sess __unused,
				      void *buf, size_t *blen)
{
	TEE_Result res;
	TEE_UUID uuid;
	const size_t nslen = 5;
	uint8_t data[5 + FVR_DIE_ID_NUM_REGS * sizeof(uint32_t)] = {
	    'O', 'P', 'T', 'E', 'E' };

	if (*blen < sizeof(uuid)) {
		*blen = sizeof(uuid);
		return TEE_ERROR_SHORT_BUFFER;
	}
	*blen = sizeof(uuid);

	if (tee_otp_get_die_id(data + nslen, sizeof(data) - nslen))
		return TEE_ERROR_BAD_STATE;

	res = tee_hash_createdigest(TEE_ALG_SHA256, data, sizeof(data),
				    (uint8_t *)&uuid, sizeof(uuid));
	if (res != TEE_SUCCESS)
		return TEE_ERROR_BAD_STATE;

	 

	uuid.timeHiAndVersion &= 0x0fff;
	uuid.timeHiAndVersion |= 5 << 12;

	 
	uuid.clockSeqAndNode[0] &= 0x3f;
	uuid.clockSeqAndNode[0] |= 0x80;

	return tee_svc_copy_to_user(buf, &uuid, sizeof(TEE_UUID));
}