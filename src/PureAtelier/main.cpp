/**
 * Copyright (c) 2024, ansonngg
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "Core/Application.h"

namespace PureAtelier
{
int Main(HINSTANCE hInstance)
{
    auto& app = Application::GetInstance();
    app.Initialize(hInstance, 1280, 720, L"Pure Atelier");
    app.Start();

    while (!app.ShouldClose())
    {
        app.Update();
    }

    return app.Close();
}
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int)
{
    return PureAtelier::Main(hInstance);
}
