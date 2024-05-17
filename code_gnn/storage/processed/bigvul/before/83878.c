static inline u32 unimac_mdio_readl(struct unimac_mdio_priv *priv, u32 offset)
{
	 
	if (IS_ENABLED(CONFIG_MIPS) && IS_ENABLED(CONFIG_CPU_BIG_ENDIAN))
		return __raw_readl(priv->base + offset);
	else
		return readl_relaxed(priv->base + offset);
}
