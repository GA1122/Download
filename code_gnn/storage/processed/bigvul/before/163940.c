PaymentHandlerWebFlowViewController::CreateHeaderContentView() {
  const GURL origin = web_contents()
                          ? web_contents()->GetVisibleURL().GetOrigin()
                          : target_.GetOrigin();
  std::unique_ptr<views::Background> background = GetHeaderBackground();
  return std::make_unique<ReadOnlyOriginView>(
      GetPaymentHandlerDialogTitle(web_contents(), https_prefix_), origin,
      state()->selected_instrument()->icon_image_skia(),
      background->get_color(), this);
}
