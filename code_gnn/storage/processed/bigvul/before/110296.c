  pp::Var GetSelectedText(bool html) {
    if (ppp_selection_ != NULL) {
      PP_Var var = ppp_selection_->GetSelectedText(plugin_->pp_instance(),
                                                   PP_FromBool(html));
      return pp::Var(pp::PASS_REF, var);
    }
    return pp::Var();
  }
