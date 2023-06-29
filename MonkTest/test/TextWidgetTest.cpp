#include "pch.h"

#include "TextWidget.h"

TEST(TextWidgetTest, CanConstruct)
{
    auto& text_widget = TextWidget("Hello");

    EXPECT_EQ(text_widget.get_text(), "");
}

TEST(TextWidgetTest, CanUpdateText)
{
    auto& text_widget = TextWidget("Hello");
    text_widget.update_text("World");

    EXPECT_EQ(text_widget.get_text(), "World");
}