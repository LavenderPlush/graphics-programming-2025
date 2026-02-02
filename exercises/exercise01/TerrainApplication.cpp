#include "TerrainApplication.h"

// (todo) 01.1: Include the libraries you need
#define STB_PERLIN_IMPLEMENTATION
#include <stb_perlin.h>
#include <cmath>
#include <iostream>
#include <vector>

// Helper structures. Declared here only for this exercise
struct Vector2
{
    Vector2() : Vector2(0.f, 0.f) {}
    Vector2(float x, float y) : x(x), y(y) {}
    float x, y;
};

struct Vector3
{
    Vector3() : Vector3(0.f,0.f,0.f) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
    float x, y, z;

    Vector3 Normalize() const
    {
        float length = std::sqrt(1 + x * x + y * y);
        return Vector3(x / length, y / length, z / length);
    }
};

// (todo) 01.8: Declare an struct with the vertex format



TerrainApplication::TerrainApplication()
    : Application(1024, 1024, "Terrain demo"), m_gridX(64), m_gridY(64), m_shaderProgram(0), m_vao(VertexArrayObject()), m_vbo(VertexBufferObject())
{
}

void TerrainApplication::Initialize()
{
    Application::Initialize();

    // Build shaders and store in m_shaderProgram
    BuildShaders();

    // (todo) 01.1: Create containers for the vertex position
    std::vector<Vector3> vertices;
    std::vector<Vector2> textureCoordinates;
    std::vector<Vector3> colors;
    std::vector<unsigned int> indices;


    // (todo) 01.1: Fill in vertex data
    for (int y = 0; y < m_gridY + 1; y++) {
        for (int x = 0; x < m_gridX + 1; x++) {
            float scaledX = (float) x / m_gridX - 0.5f;
            float scaledY = (float) y / m_gridY - 0.5f;

            float frequency = 0.15;
            float z = 0.0;
            float lacunarity = 10.0;
            float gain = 0.1;
            int octaves = 1;
            float magnitude = 0.2;

            float scaledZ = stb_perlin_fbm_noise3(x * frequency, y * frequency, z, lacunarity, gain, octaves) * magnitude;
            Vector3 vertex = Vector3(scaledX, scaledY, scaledZ);

            vertices.push_back(vertex);
            textureCoordinates.push_back(Vector2(x, y));

            float oceanThreshold = -0.02;
            float sandThreshold = 0.02;
            float grassThreshold = 0.045;

            Vector3 oceanColor = Vector3(0.3, 0.45, 0.6);
            Vector3 sandColor = Vector3(0.8, 0.7, 0.5);
            Vector3 grassColor = Vector3(0.35, 0.55, 0.4);
            Vector3 mountainColor = Vector3(0.5, 0.5, 0.5);

            Vector3* chosenColor;

            if (scaledZ <= oceanThreshold) {
                chosenColor = &oceanColor;
            } else if (scaledZ <= sandThreshold) {
                chosenColor = &sandColor;
            } else if (scaledZ <= grassThreshold) {
                chosenColor = &grassColor;
            } else {
                chosenColor = &mountainColor;
            }

            colors.push_back(*chosenColor);

            if (y < m_gridY && x < m_gridX) {
                indices.push_back(y * (m_gridX + 1) + x);
                indices.push_back(y * (m_gridX + 1) + x + 1);
                indices.push_back((y + 1) * (m_gridX + 1) + x);

                indices.push_back(y * (m_gridX + 1) + x + 1);
                indices.push_back((y + 1) * (m_gridX + 1) + x);
                indices.push_back((y + 1) * (m_gridX + 1) + x + 1);
            }
        }
    }

    // (todo) 01.1: Initialize VAO, and VBO
    m_vao.Bind();
    m_vbo.Bind();

    std::span verticesSpan = std::span(vertices.data(), vertices.size());
    std::span textureCoordinatesSpan = std::span(textureCoordinates.data(), textureCoordinates.size());
    std::span colorSpan = std::span(colors.data(), colors.size());

    m_vbo.AllocateData(verticesSpan.size() * sizeof(Vector3) + textureCoordinatesSpan.size() * sizeof(Vector2) + colors.size() * sizeof(Vector3), VertexBufferObject::StaticDraw);

    m_vbo.UpdateData(verticesSpan, 0);
    m_vbo.UpdateData(textureCoordinatesSpan, verticesSpan.size() * sizeof(Vector3));
    m_vbo.UpdateData(colorSpan, verticesSpan.size() * sizeof(Vector3) + textureCoordinatesSpan.size() * sizeof(Vector2));

    VertexAttribute verticesAttribute = VertexAttribute(Data::Type::Float, 3);
    m_vao.SetAttribute(0, verticesAttribute, 0);

    VertexAttribute textureCoordinatesAttribute = VertexAttribute(Data::Type::Float, 2);
    m_vao.SetAttribute(1, textureCoordinatesAttribute, verticesSpan.size() * sizeof(Vector3));

    VertexAttribute colorAttribute = VertexAttribute(Data::Type::Float, 3);
    m_vao.SetAttribute(2, colorAttribute, verticesSpan.size() * sizeof(Vector3) + textureCoordinatesSpan.size() * sizeof(Vector2));

    // (todo) 01.5: Initialize EBO
    std::span indicesSpan = std::span(indices.data(), indices.size());
    m_ebo.Bind();
    m_ebo.AllocateData(indicesSpan);

    // (todo) 01.1: Unbind VAO, and VBO
    m_vbo.Unbind();
    m_vao.Unbind();

    // (todo) 01.5: Unbind EBO
    m_ebo.Unbind();

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_DEPTH_TEST);
}

void TerrainApplication::Update()
{
    Application::Update();

    UpdateOutputMode();
}

void TerrainApplication::Render()
{
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    Application::Render();

    // Clear color and depth
    GetDevice().Clear(true, Color(0.0f, 0.0f, 0.0f, 1.0f), true, 1.0f);

    // Set shader to be used
    glUseProgram(m_shaderProgram);

    // (todo) 01.1: Draw the grid
    m_vao.Bind();
    // glDrawArrays(GL_TRIANGLES, 0, m_gridX * m_gridY * 6);
    glDrawElements(GL_TRIANGLES, 2 * 3 * m_gridX * m_gridY, GL_UNSIGNED_INT, 0);
}

void TerrainApplication::Cleanup()
{
    Application::Cleanup();
}


void TerrainApplication::BuildShaders()
{
    const char* vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec2 aTexCoord;\n"
        "layout (location = 2) in vec3 aColor;\n"
        "layout (location = 3) in vec3 aNormal;\n"
        "uniform mat4 Matrix = mat4(1);\n"
        "out vec2 texCoord;\n"
        "out vec3 color;\n"
        "out vec3 normal;\n"
        "void main()\n"
        "{\n"
        "   texCoord = aTexCoord;\n"
        "   color = aColor;\n"
        "   normal = aNormal;\n"
        "   gl_Position = Matrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";
    const char* fragmentShaderSource = "#version 330 core\n"
        "uniform uint Mode = 0u;\n"
        "in vec2 texCoord;\n"
        "in vec3 color;\n"
        "in vec3 normal;\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   switch (Mode)\n"
        "   {\n"
        "   default:\n"
        "   case 0u:\n"
        "       FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
        "       break;\n"
        "   case 1u:\n"
        "       FragColor = vec4(fract(texCoord), 0.0f, 1.0f);\n"
        "       break;\n"
        "   case 2u:\n"
        "       FragColor = vec4(color, 1.0f);\n"
        "       break;\n"
        "   case 3u:\n"
        "       FragColor = vec4(normalize(normal), 1.0f);\n"
        "       break;\n"
        "   case 4u:\n"
        "       FragColor = vec4(color * max(dot(normalize(normal), normalize(vec3(1,0,1))), 0.2f), 1.0f);\n"
        "       break;\n"
        "   }\n"
        "}\n\0";

    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    m_shaderProgram = shaderProgram;
}

void TerrainApplication::UpdateOutputMode()
{
    for (int i = 0; i <= 4; ++i)
    {
        if (GetMainWindow().IsKeyPressed(GLFW_KEY_0 + i))
        {
            int modeLocation = glGetUniformLocation(m_shaderProgram, "Mode");
            glUseProgram(m_shaderProgram);
            glUniform1ui(modeLocation, i);
            break;
        }
    }
    if (GetMainWindow().IsKeyPressed(GLFW_KEY_TAB))
    {
        const float projMatrix[16] = { 0, -1.294f, -0.721f, -0.707f, 1.83f, 0, 0, 0, 0, 1.294f, -0.721f, -0.707f, 0, 0, 1.24f, 1.414f };
        int matrixLocation = glGetUniformLocation(m_shaderProgram, "Matrix");
        glUseProgram(m_shaderProgram);
        glUniformMatrix4fv(matrixLocation, 1, false, projMatrix);
    }
}
