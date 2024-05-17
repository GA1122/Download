_gradient_walker_init (GradientWalker  *walker,
		       SourcePictPtr    pGradient,
		       unsigned int     spread)
{
    walker->num_stops = pGradient->gradient.nstops;
    walker->stops     = pGradient->gradient.stops;
    walker->left_x    = 0;
    walker->right_x   = 0x10000;
    walker->stepper   = 0;
    walker->left_ag   = 0;
    walker->left_rb   = 0;
    walker->right_ag  = 0;
    walker->right_rb  = 0;
    walker->spread    = spread;

    walker->need_reset = TRUE;
}
