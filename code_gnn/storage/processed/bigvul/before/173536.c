int set_config(effect_context_t *context, effect_config_t *config)
{
    context->config = *config;

 if (context->ops.reset)
        context->ops.reset(context);

 return 0;
}
