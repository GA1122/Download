SizeID XRRConfigCurrentConfiguration (XRRScreenConfiguration *config,
			      Rotation *rotation)
{
    *rotation = (Rotation) config->current_rotation;
    return (SizeID) config->current_size;
}
