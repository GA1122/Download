void LayerTreeHost::CalculateLCDTextMetricsCallback(Layer* layer) {
  if (!layer->SupportsLCDText())
    return;

  lcd_text_metrics_.total_num_cc_layers++;
  if (layer->draw_properties().can_use_lcd_text) {
    lcd_text_metrics_.total_num_cc_layers_can_use_lcd_text++;
    if (layer->contents_opaque())
      lcd_text_metrics_.total_num_cc_layers_will_use_lcd_text++;
  }
}
