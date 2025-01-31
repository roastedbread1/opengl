#include "pch.h"
#include "lightning.h"
#if __has_include("lightning.g.cpp")
#include "lightning.g.cpp"
#endif

using namespace winrt;
using namespace Windows::UI::Xaml;

namespace winrt::breadman::implementation
{
    int32_t lightning::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void lightning::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    void lightning::ClickHandler(IInspectable const&, RoutedEventArgs const&)
    {
        Button().Content(box_value(L"Clicked"));
    }
}
