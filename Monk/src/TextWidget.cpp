#include "TextWidget.h"

void TextWidget::setup()
{
}

void TextWidget::render()
{
	ImGui::InputText(m_header.c_str(), &m_content);
}

void TextWidget::update_text(const char* content)
{
	m_content = std::string{ content };
}

const std::string& TextWidget::get_text() const
{
	return m_content;
}

const char* TextWidget::get_text_modifiable() const
{
	return m_content.c_str();
}