#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "fileutils.cpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// render
int VBOTriangle1();
int VBOTriangle2();
int moreVboAndVao();
int EBORectangle();
int TriangleExercise1();

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int windowInit();

// opengl
GLuint compileShader(const char*, const int);

int success;
char infoLog[512];

GLFWwindow* window;

int main()
{
  if (windowInit() != 0) return -1;
  int status = TriangleExercise1();
  // int status = EBORectangle();

  // glfw: terminate, clearing all previously allocated GLFW resources.
  // ------------------------------------------------------------------
  glfwTerminate();
  return status;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

int windowInit() {
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
      glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    // glfw window creation
    // --------------------
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    return 0;
}

GLuint compileShader(const char* shaderSource, int shaderType) {
  GLuint shader = glCreateShader(shaderType);
  glShaderSource(shader, 1, &shaderSource, NULL);
  glCompileShader(shader);

  // compilation checks
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    throw std::runtime_error("Error compiling shader: " + (std::string) infoLog + "\n");
  }
  return shader;
}

int VBOTriangle(const float vertices[], const size_t vertSize) {
  const char* vertexShaderFilePath = "shaders/vertex_shader.glsl";
  const char* fragmentShaderFilePath = "shaders/fragment_shader.glsl";

  GLuint VAO;
  GLuint VBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertSize, vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  const char* vertexShaderSource = readFileToCharPointer(vertexShaderFilePath);
  const char* fragmentShaderSource = readFileToCharPointer(fragmentShaderFilePath);

  GLuint vertexShader;
  GLuint fragmentShader;

  try {
    vertexShader = compileShader(vertexShaderSource, GL_VERTEX_SHADER);
    fragmentShader = compileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);
  } catch (const std::runtime_error& e) {
    std::cout << "An error occured. " << e.what() << "\n";
    return -1;
  }

  GLuint shaderProgram = glCreateProgram();

  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  // compilation checks
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    throw std::runtime_error("Error linking shaders: " + (std::string) infoLog + "\n");
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  while(!glfwWindowShouldClose(window)) {
    processInput(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);

    glDrawArrays(GL_TRIANGLES, 0, vertSize / 3);

    glBindVertexArray(0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  return 0;
}

int TriangleExercise1() {
  const float vertices1[] = {
    // left triangle
    -0.5f, 0.5f, 0.0f,
    -1.0f, -0.5f, 0.0f,
    0.0f, -0.5f, 0.0f,
  };

  const float vertices2[] = {
    // right triangle
    0.0f, -0.5f, 0.0f,
    0.5f, 0.5f, 0.0f,
    1.0f, -0.5f, 0.0f,
  };

  const char* vertexShaderFilePath = "shaders/vertex_shader.glsl";
  const char* fragmentShaderFilePath1 = "shaders/fragment_shader.glsl";
  const char* fragmentShaderFilePath2 = "shaders/fragment_shader_1.glsl";

  GLuint VAO[2];
  GLuint VBO[2];
  glGenVertexArrays(2, VAO);
  glGenBuffers(2, VBO);

  // left triangle
  glBindVertexArray(VAO[0]);
  glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // right triangle
  glBindVertexArray(VAO[1]);
  glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  const char* vertexShaderSource = readFileToCharPointer(vertexShaderFilePath);
  const char* fragmentShaderSource1 = readFileToCharPointer(fragmentShaderFilePath1);
  const char* fragmentShaderSource2 = readFileToCharPointer(fragmentShaderFilePath2);

  GLuint vertexShader;
  GLuint fragmentShader1;
  GLuint fragmentShader2;

  try {
    vertexShader = compileShader(vertexShaderSource, GL_VERTEX_SHADER);
    fragmentShader1 = compileShader(fragmentShaderSource1, GL_FRAGMENT_SHADER);
    fragmentShader2 = compileShader(fragmentShaderSource2, GL_FRAGMENT_SHADER);
  } catch (const std::runtime_error& e) {
    std::cout << "An error occured. " << e.what() << "\n";
    return -1;
  }

  GLuint shaderProgram1 = glCreateProgram();
  GLuint shaderProgram2 = glCreateProgram();

  // left triangle shader prog
  glAttachShader(shaderProgram1, vertexShader);
  glAttachShader(shaderProgram1, fragmentShader1);
  glLinkProgram(shaderProgram1);

  // compilation checks
  glGetProgramiv(shaderProgram1, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram1, 512, NULL, infoLog);
    throw std::runtime_error("Error linking shaders: " + (std::string) infoLog + "\n");
  }

  // right triangle shader prog
  glAttachShader(shaderProgram2, vertexShader);
  glAttachShader(shaderProgram2, fragmentShader2);
  glLinkProgram(shaderProgram2);

  // compilation checks
  glGetProgramiv(shaderProgram2, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram2, 512, NULL, infoLog);
    throw std::runtime_error("Error linking shaders: " + (std::string) infoLog + "\n");
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader1);
  glDeleteShader(fragmentShader2);

  while(!glfwWindowShouldClose(window)) {
    processInput(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // left triangle draws
    glUseProgram(shaderProgram1);
    glBindVertexArray(VAO[0]);

    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices1) / 3);

    glBindVertexArray(0);

    // right triangle draws
    glUseProgram(shaderProgram2);
    glBindVertexArray(VAO[1]);

    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices2) / 3);

    glBindVertexArray(0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  return 0;
}

int VBOTriangle1() {
  const float vertices[] = {
    0.0f, 0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f
  };

  return VBOTriangle(vertices, sizeof(vertices));
}

int VBOTriangle2() {
  const float vertices[] = {
    // left triangle
    -0.5f, 0.5f, 0.0f,
    -1.0f, -0.5f, 0.0f,
    0.0f, -0.5f, 0.0f,

    // right triangle
    0.0f, -0.5f, 0.0f,
    0.5f, 0.5f, 0.0f,
    1.0f, -0.5f, 0.0f,
  };

  return VBOTriangle(vertices, sizeof(vertices));
}

int EBORectangle() {
  const char* vertexShaderFilePath = "shaders/vertex_shader.glsl";
  const char* fragmentShaderFilePath = "shaders/fragment_shader.glsl";

  // optimized vertex draw using EBO
  const float vertices[] = {
     0.5f,  0.5f,  0.0f,
     0.5f, -0.5f,  0.0f,
    -0.5f,  0.5f,  0.0f,
    -0.5f, -0.5f,  0.0f,
  };

  const int indices[] = {
    0, 1, 2, // first triangle vertices position
    3, 1, 2, // second triangle vertices positions
  };

  // opengl initialization
  GLuint VAO;
  GLuint VBO;
  GLuint EBO;
  glGenBuffers(1, &VBO);
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // shader inits and compilation

  const char* vertexShaderSource = readFileToCharPointer(vertexShaderFilePath);
  const char* fragmentShaderSource = readFileToCharPointer(fragmentShaderFilePath);

  GLuint vertexShader;
  GLuint fragmentShader;

  try {
    vertexShader = compileShader(vertexShaderSource, GL_VERTEX_SHADER);
    fragmentShader = compileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);
  } catch (const std::runtime_error& e) {
    std::cout << "An error occured. " << e.what() << "\n";
    return -1;
  }

  GLuint shaderProgram = glCreateProgram();

  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  // compilation checks
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    throw std::runtime_error("Error linking shaders: " + (std::string) infoLog + "\n");
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  // render loop
  // -----------
  while (!glfwWindowShouldClose(window))
  {
    // input
    // -----
    processInput(window);

    // render
    // ------
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);

    // vertices method for drawing
    // glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / 3);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // indexed drawing via EBO
    glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);

    // to unbind, use address of 0
    glBindVertexArray(0);
    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  return 0;
}
