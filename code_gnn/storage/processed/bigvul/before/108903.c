static bool IsReload(const ViewMsg_Navigate_Params& params) {
  return
      params.navigation_type == ViewMsg_Navigate_Type::RELOAD ||
      params.navigation_type == ViewMsg_Navigate_Type::RELOAD_IGNORING_CACHE ||
      params.navigation_type ==
          ViewMsg_Navigate_Type::RELOAD_ORIGINAL_REQUEST_URL;
}
