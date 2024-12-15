#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

// Vertex data for a cube
float vertices[] = {
    // Positions          // Colors
   -0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f, // Front-bottom-left
    0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f, // Front-bottom-right
    0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 1.0f, // Front-top-right
   -0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f, // Front-top-left
   -0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 1.0f, // Back-bottom-left
    0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 1.0f, // Back-bottom-right
    0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 1.0f, // Back-top-right
   -0.5f,  0.5f,  0.5f,   0.5f, 0.5f, 0.5f  // Back-top-left
};

// Indices to draw the cube
GLuint indices[] = {
    0, 1, 2,  2, 3, 0, // Front face
    4, 5, 6,  6, 7, 4, // Back face
    4, 5, 1,  1, 0, 4, // Bottom face
    3, 2, 6,  6, 7, 3, // Top face
    0, 3, 7,  7, 4, 0, // Left face
    1, 5, 6,  6, 2, 1  // Right face
};

GLuint VBO, VAO, EBO, shaderProgram;

const char* vertexShaderSource = R"(
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;

out vec3 ourColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
    ourColor = aColor;
}
)";

const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;

in vec3 ourColor;

void main()
{
    FragColor = vec4(ourColor, 1.0f);
}
)";

// Setup OpenGL buffers and shaders
void setup() {
    // Compile vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    // Compile fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    // Link shaders to a shader program
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Generate and bind VAO, VBO, and EBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // Load vertex data into VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Load index data into EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Set vertex attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); // Positions
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); // Colors
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// Callback function to adjust viewport size
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// Process input to close window (ESC key)
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}


int main() {
    // Initialize GLFW
    if (!glfwInit()) return -1;

    // Create windowed mode window and OpenGL context
    GLFWwindow* window = glfwCreateWindow(800, 600, "Spinning Cube", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Load OpenGL function pointers using GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    // Setup OpenGL buffers and shaders
    setup();

    // Define transformation matrices
    glm::mat4 model = glm::mat4(1.0f); // Identity matrix
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f)); // Move the cube back

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 100.0f); // Perspective projection

    // Time variables
    float lastFrame = 0.0f;  // Time of the last frame
    float deltaTime = 0.0f;  // Time between current and last frame

    while (!glfwWindowShouldClose(window)) {
        // Calculate deltaTime
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Input handling
        processInput(window);

        // Render
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers

        // Use the shader program
        glUseProgram(shaderProgram);

        // Get uniform locations
        GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
        GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
        GLuint projLoc = glGetUniformLocation(shaderProgram, "projection");

        // Update the model matrix for rotation
        float rotationSpeed = 1.0f; // Set the desired rotation speed
        model = glm::rotate(model, rotationSpeed * deltaTime, glm::vec3(0.5f, 1.0f, 0.0f)); // Rotate the cube based on time
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // Render the cube
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0); // Draw cube with indices

        // Swap buffers
        glfwSwapBuffers(window);

        // Poll events
        glfwPollEvents();
    }


    // Clean up and terminate
    glfwTerminate();
    return 0;
}