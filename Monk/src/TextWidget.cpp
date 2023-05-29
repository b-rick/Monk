#include "TextWidget.h"

void TextWidget::setup()
{
}

void TextWidget::render()
{
	ImGui::InputText(header.c_str(), &content);
}

const std::string& TextWidget::get_text() const
{
	return content;
}

const char* TextWidget::get_text_modifiable() const
{
	return content.c_str();
}