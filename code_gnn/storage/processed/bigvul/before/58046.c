int nft_validate_input_register(enum nft_registers reg)
{
	if (reg <= NFT_REG_VERDICT)
		return -EINVAL;
	if (reg > NFT_REG_MAX)
		return -ERANGE;
	return 0;
}