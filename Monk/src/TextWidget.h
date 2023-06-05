#pragma once

#include <string>
#include "Widget.h"

class TextWidget : Widget
{
private:
	std::string m_content;
	const std::string m_header;

public:
	TextWidget(const char* header_text) : m_header{ std::string{header_text} }
	{
	}

	~TextWidget() override {};

	// Inherited via Widget
	virtual void setup() override;

	virtual void render() override;

	void update_text(const char* text);

	const std::string& get_text() const;

	const char* get_text_modifiable() const;
};
