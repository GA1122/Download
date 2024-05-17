Rotation XRRConfigRotations(XRRScreenConfiguration *config, Rotation *current_rotation)
{
  *current_rotation = config->current_rotation;
  return config->rotations;
}
