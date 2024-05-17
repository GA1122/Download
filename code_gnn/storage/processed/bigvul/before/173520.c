void add_effect_to_output(output_context_t * output, effect_context_t *context)
{
 struct listnode *fx_node;

    list_for_each(fx_node, &output->effects_list) {
 effect_context_t *fx_ctxt = node_to_item(fx_node,
 effect_context_t,
                                                 output_node);
 if (fx_ctxt == context)
 return;
 }
    list_add_tail(&output->effects_list, &context->output_node);
 if (context->ops.start)
        context->ops.start(context, output);

}
