  gfx::Rect GetTransformedBoundsInRootWindow(aura::Window* window) {
    gfx::RectF bounds = gfx::RectF(gfx::SizeF(window->bounds().size()));
    aura::Window* root = window->GetRootWindow();
    CHECK(window->layer());
    CHECK(root->layer());
    gfx::Transform transform;
    if (!window->layer()->GetTargetTransformRelativeTo(root->layer(),
                                                       &transform)) {
      return gfx::Rect();
    }
    transform.TransformRect(&bounds);
    return gfx::ToEnclosingRect(bounds);
  }
