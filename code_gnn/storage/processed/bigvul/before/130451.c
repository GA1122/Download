  static std::string ID() {
    update_client::CrxComponent component;
    component.version = Version("0.0.0.0");
    GetPkHash(&component.pk_hash);
    return update_client::GetCrxComponentID(component);
  }
