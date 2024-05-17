bool Document::IsInInvisibleSubframe() const {
  if (!LocalOwner())
    return false;   

  DCHECK(GetFrame());
  return GetFrame()->OwnerLayoutItem().IsNull();
}
