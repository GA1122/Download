void Gfx::popStateGuard() {
    while (stackHeight > bottomGuard() && state->hasSaves())
	restoreState();
    stateGuards.pop_back();
}
