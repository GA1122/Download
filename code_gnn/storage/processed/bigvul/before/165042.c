void HTMLCanvasElement::StyleDidChange(const ComputedStyle* old_style,
                                       const ComputedStyle& new_style) {
  if (context_)
    context_->StyleDidChange(old_style, new_style);
}
