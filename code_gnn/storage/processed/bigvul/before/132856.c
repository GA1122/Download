void PictureLayerImpl::SanityCheckTilingState() const {
#if DCHECK_IS_ON
  if (layer_tree_impl()->IsRecycleTree())
    return;

  if (!CanHaveTilings()) {
    DCHECK_EQ(0u, tilings_->num_tilings());
    return;
  }
  if (tilings_->num_tilings() == 0)
    return;

  DCHECK_EQ(1, tilings_->NumHighResTilings());
#endif
}
