#pragma once

#include "lightning.g.h"

namespace winrt::breadman::implementation
{
    struct lightning : lightningT<lightning>
    {
        lightning() 
        {
            // Xaml objects should not call InitializeComponent during construction.
            // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent
        }

        int32_t MyProperty();
        void MyProperty(int32_t value);

        void ClickHandler(Windows::Foundation::IInspectable const& sender, Windows::UI::Xaml::RoutedEventArgs const& args);
    };
}

namespace winrt::breadman::factory_implementation
{
    struct lightning : lightningT<lightning, implementation::lightning>
    {
    };
}
