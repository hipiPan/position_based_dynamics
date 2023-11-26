#pragma once

class Renderer;

class DebugPass
{
public:
    DebugPass(Renderer* renderer);

    void render();

private:
    Renderer* _renderer;
};