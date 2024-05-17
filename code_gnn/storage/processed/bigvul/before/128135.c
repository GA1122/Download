bool TrayCast::HasCastExtension() {
  return cast_config_delegate_ != nullptr &&
         cast_config_delegate_->HasCastExtension();
}
