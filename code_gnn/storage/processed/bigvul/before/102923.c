DefaultTabHandler::DefaultTabHandler(TabHandlerDelegate* delegate)
    : delegate_(delegate),
      ALLOW_THIS_IN_INITIALIZER_LIST(
          model_(new TabStripModel(this, delegate->GetProfile()))) {
  UmaNaclHistogramEnumeration(FIRST_TAB_NACL_BASELINE);
  model_->AddObserver(this);
}
