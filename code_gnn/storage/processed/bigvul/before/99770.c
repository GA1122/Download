void Vp9Parser::ReadQuantization(Vp9QuantizationParams* quants) {
  quants->base_qindex = reader_.ReadLiteral(8);

  if (reader_.ReadBool())
    quants->y_dc_delta = reader_.ReadSignedLiteral(4);

  if (reader_.ReadBool())
    quants->uv_ac_delta = reader_.ReadSignedLiteral(4);

  if (reader_.ReadBool())
     quants->uv_dc_delta = reader_.ReadSignedLiteral(4);
 }
