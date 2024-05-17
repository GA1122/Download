void GuestViewBase::DispatchOnResizeEvent(const gfx::Size& old_size,
                                          const gfx::Size& new_size) {
  if (new_size == old_size)
    return;

  scoped_ptr<base::DictionaryValue> args(new base::DictionaryValue());
  args->SetInteger(guestview::kOldWidth, old_size.width());
  args->SetInteger(guestview::kOldHeight, old_size.height());
  args->SetInteger(guestview::kNewWidth, new_size.width());
  args->SetInteger(guestview::kNewHeight, new_size.height());
  DispatchEventToGuestProxy(
      new GuestViewEvent(guestview::kEventResize, args.Pass()));
}
