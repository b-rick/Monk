#include "pch.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "AutoShader.h"
#include "Widget.h"
#include "TextWidget.h"

const char* vertex_shader_good = 
R""""(
#version 330 core

layout (location = 0) in vec3 aPos;

void main()
{
	gl_Position = vec4(aPos, 1.0);
}	
)"""";

const char* frag_shader_good =
R""""(
#version 330 core

out vec4 FragColor;

void main()
{
	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}
)"""";

const char* vertex_shader_bad =
R""""(
#version 330 core

layout (location = 0) in vec3 aPos

void main()
{
	gl_Position = vec4(aPos, 1.0)
}	
)"""";

static GLFWwindow* init_test()
{
    if (!glfwInit())
    {
        std::cerr << "Failed to init glfw" << std::endl;
        return nullptr;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window = glfwCreateWindow(10, 10, "Test", NULL, NULL);

    if (!window)
    {
        std::cerr << "Failed to create glfw window" << std::endl;
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);
    if (!gladLoadGL())
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return nullptr;
    }

    return window;
}

static void close_test(GLFWwindow* window)
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

TEST(AutoShaderTest, ShaderInvalidOnStartup)
{
    auto window = init_test();
    if (window == nullptr)
    {
        ASSERT_FALSE(true);
    }

    auto& auto_shader = AutoShader(vertex_shader_good, frag_shader_good);

    EXPECT_FALSE(auto_shader.get_status());

    close_test(window);
}

TEST(AutoShaderTest, CanCompileValidShader) 
{
    auto window = init_test();
    if (window == nullptr)
    {
        ASSERT_FALSE(true);
    }

	auto& auto_shader = AutoShader(vertex_shader_good, frag_shader_good );
	auto_shader.try_compile();

	EXPECT_TRUE(auto_shader.get_status());

    close_test(window);
}

TEST(AutoShaderTest, ContinuesOnInvalidShader)
{
    auto window = init_test();
    if (window == nullptr)
    {
        ASSERT_FALSE(true);
    }

    auto& auto_shader = AutoShader(vertex_shader_bad, frag_shader_good);
    auto_shader.try_compile();

    EXPECT_FALSE(auto_shader.get_status());

    close_test(window);
}

TEST(AutoShaderTest, CanMoveConstructAutoShader)
{
    auto window = init_test();
    if (window == nullptr)
    {
        ASSERT_FALSE(true);
    }

    auto vert_widget = std::make_unique<TextWidget>(vertex_shader_bad);
    auto frag_widget = std::make_unique<TextWidget>(frag_shader_good);

    auto vert_ptr = vert_widget.get();
    auto frag_ptr = frag_widget.get();

    auto& auto_shader = AutoShader{ std::move(vert_widget), std::move(frag_widget) };
    EXPECT_FALSE(auto_shader.get_status());

    close_test(window);
}

TEST(AutoShaderTest, CanCompileAfterFailure)
{
    auto window = init_test();
    if (window == nullptr)
    {
        ASSERT_FALSE(true);
    }

    auto vert_widget = std::make_unique<TextWidget>(vertex_shader_bad);
    auto frag_widget = std::make_unique<TextWidget>(frag_shader_good);

    auto vert_ptr = vert_widget.get();
    auto frag_ptr = frag_widget.get();

    auto& auto_shader = AutoShader{ std::move(vert_widget), std::move(frag_widget) };
    EXPECT_FALSE(auto_shader.get_status());

    auto_shader.try_compile();
    EXPECT_FALSE(auto_shader.get_status());

    vert_ptr->update_text(vertex_shader_good);
    auto_shader.try_compile();
    EXPECT_TRUE(auto_shader.get_status());

    close_test(window);
}