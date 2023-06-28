#include "DebugWindow.h"

void DebugWindow::open(bool open)
{
	m_open = open;
}

const char* DebugWindow::calc_line_end(const char* buf, int line_no, const std::vector<int>& line_offsets)
{
	return (line_no + 1 < line_offsets.size()) ? (buf + line_offsets[line_no + 1] - 1) : buf;
}

void DebugWindow::render()
{
	if (!m_open)
	{
		return;
	}
	if (!ImGui::Begin(m_title, &m_open))
	{
		ImGui::End();
		return;
	}
	ImGui::Text("%.3f FPS | %.3f ms/F", ImGui::GetIO().Framerate, 1000.0f / ImGui::GetIO().Framerate);
	ImGui::SameLine();
	auto should_clear = ImGui::Button("Clear");
	ImGui::SameLine();
	ImGui::Checkbox("Paused", &m_paused); // Todo: implement log pausing
	ImGui::SameLine();
	m_filter.Draw("Filter", -100.0f);
	
	ImGui::Separator();
	if (ImGui::BeginChild("scrolling", ImVec2(0, 0), m_border_shown, ImGuiWindowFlags_HorizontalScrollbar))
	{
		if (should_clear)
		{
			clear();
		}

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
		const char* buf = m_buffer.begin();
		const char* buf_end = m_buffer.end();
		
		if (m_filter.IsActive())
		{
			for (int line_no = 0; line_no < m_line_offsets.size(); line_no++)
			{
				const char* line_start = buf + m_line_offsets[line_no];
				const char* line_end = calc_line_end(buf, line_no, m_line_offsets);
				if (m_filter.PassFilter(line_start, line_end))
				{
					ImGui::TextUnformatted(line_start, line_end);
				}
			}
		}
		else
		{
			ImGuiListClipper clipper;
			clipper.Begin(static_cast<int>(m_line_offsets.size()));
			while (clipper.Step())
			{
				for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
				{
					const char* line_start = buf + m_line_offsets[line_no];
					const char* line_end = calc_line_end(buf, line_no, m_line_offsets);
					ImGui::TextUnformatted(line_start, line_end);
				}
			}
			clipper.End();
		}
		ImGui::PopStyleVar();
		if (m_auto_scroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
		{
			ImGui::SetScrollHereY(1.0f);
		}
	}
	ImGui::EndChild();
	ImGui::End();
}


void DebugWindow::clear()
{
	m_buffer.clear();
	m_line_offsets.clear();
	m_line_offsets.push_back(0);
	m_paused = false;
}

void DebugWindow::add_log(const char* fmt, va_list args)
{
	auto old_size = m_buffer.size();
	m_buffer.appendfv(fmt, args);

	for (int new_size = m_buffer.size(); old_size < new_size; old_size++)
	{
		if (m_buffer[old_size] == '\n')
		{
			m_line_offsets.push_back(old_size + 1);
		}
	}
}

void DebugWindow::log_info(const char* fmt, ...) IM_FMTARGS(2)
{
	auto prefix = "[%05d:%.1f] [%s] ";
	m_buffer.appendf(prefix, ImGui::GetFrameCount(), ImGui::GetTime(), "info");

	va_list args;
	va_start(args, fmt);
	add_log(fmt, args);
	va_end(args);
}

void DebugWindow::log_warn(const char* fmt, ...) IM_FMTARGS(2)
{
	auto prefix = "[%05d:%.1f] [%s] ";
	m_buffer.appendf(prefix, ImGui::GetFrameCount(), ImGui::GetTime(), "warn");

	va_list args;
	va_start(args, fmt);
	add_log(fmt, args);
	va_end(args);
}

void DebugWindow::log_error(const char* fmt, ...) IM_FMTARGS(2)
{
	auto prefix = "[%05d:%.1f] [%s] ";
	m_buffer.appendf(prefix, ImGui::GetFrameCount(), ImGui::GetTime(), "error");

	va_list args;
	va_start(args, fmt);
	add_log(fmt, args);
	va_end(args);
}

void DebugWindow::log_debug(const char* fmt, ...) IM_FMTARGS(2)
{
	auto prefix = "[%05d:%.1f] [%s] ";
	m_buffer.appendf(prefix, ImGui::GetFrameCount(), ImGui::GetTime(), "debug");

	va_list args;
	va_start(args, fmt);
	add_log(fmt, args);
	va_end(args);
}

