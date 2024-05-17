ChromeExtensionsAPIClient::GetNetworkingCastPrivateDelegate() {
#if defined(OS_CHROMEOS) || defined(OS_WIN) || defined(OS_MACOSX)
  if (!networking_cast_private_delegate_)
    networking_cast_private_delegate_ =
        ChromeNetworkingCastPrivateDelegate::Create();
#endif
  return networking_cast_private_delegate_.get();
}
