gpk_read_binary(sc_card_t *card, unsigned int offset,
		u8 *buf, size_t count, unsigned long flags)
{
	struct gpk_private_data *priv = DRVDATA(card);

	if (offset & priv->offset_mask) {
		sc_debug(card->ctx, SC_LOG_DEBUG_NORMAL, "Invalid file offset (not a multiple of %d)",
				priv->offset_mask + 1);
		return SC_ERROR_INVALID_ARGUMENTS;
	}
	return iso_ops->read_binary(card, offset >> priv->offset_shift,
			buf, count, flags);
}