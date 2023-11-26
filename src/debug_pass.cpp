#include "debug_pass.h"
#include "renderer.h"
#include <rhi/shader_manager.h>

DebugPass::DebugPass(Renderer* renderer)
{
    _renderer = renderer;
}

void DebugPass::render()
{
}