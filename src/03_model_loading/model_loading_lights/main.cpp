#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "shader.h"
#include "camera.h"
#include "model.h"
#include <ostream>
#define STB_IMAGE_IMPLEMENTATION
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <stb_image.h>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Camera camera(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f));

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

glm::vec3 pointLightPositions[] = {
    glm::vec3( 4.0f,  3.0f, 2.0f),
    glm::vec3(-4.0f,  3.0f, 2.0f),
    glm::vec3( 4.0f, -3.0f, 2.0f),
    glm::vec3(-4.0f, -3.0f, 2.0f)
};

glm::vec3 pointLightColors[] = {
    glm::vec3(1.0f, 0.6f, 0.6f),
    glm::vec3(0.6f, 1.0f, 0.6f),
    glm::vec3(0.6f, 0.6f, 1.0f),
    glm::vec3(1.0f, 1.0f, 0.4f)
};

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", nullptr, nullptr);
  if (!window) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  int version = gladLoadGL(glfwGetProcAddress);
  if (version == 0) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    glfwTerminate();
    return -1;
  }

  glEnable(GL_DEPTH_TEST);

  stbi_set_flip_vertically_on_load(true);

  Shader ourShader("src/03_model_loading/model_loading_lights/model.vs", "src/03_model_loading/model_loading_lights/model.fs");
  Model ourModel("resources/objects/backpack/backpack.obj");

  while (!glfwWindowShouldClose(window)) {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    processInput(window);

    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 projection = glm::perspective(glm::radians(camera.fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();

    ourShader.use();
    ourShader.setMat4("projection", projection);
    ourShader.setMat4("view", view);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f));
    model = glm::scale(model, glm::vec3(1.0f));
    ourShader.setMat4("model", model);
    ourShader.setVec3("viewPos", camera.position);
    ourShader.setFloat("material.shininess", 32.0f);

    // directional light
    ourShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
    ourShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
    ourShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
    ourShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

    // point lights
    for (std::size_t i = 0; i < std::size(pointLightPositions); ++i) {
        const std::string light = "pointLights[" + std::to_string(i) + "]";
        glm::vec3 ambient = pointLightColors[i] * 0.05f;
        glm::vec3 diffuse = pointLightColors[i] * 0.8f;

        ourShader.setVec3(light + ".position", pointLightPositions[i]);
        ourShader.setVec3(light + ".ambient", ambient);
        ourShader.setVec3(light + ".diffuse", diffuse);
        ourShader.setVec3(light + ".specular", pointLightColors[i]);
        ourShader.setFloat(light + ".constant", 1.0f);
        ourShader.setFloat(light + ".linear", 0.09f);
        ourShader.setFloat(light + ".quadratic", 0.032f);
    }

    // spotLight
    // ourShader.setVec3("spotLight.position", camera.position);
    // ourShader.setVec3("spotLight.direction", camera.front);
    // ourShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
    // ourShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
    // ourShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
    // ourShader.setFloat("spotLight.constant", 1.0f);
    // ourShader.setFloat("spotLight.linear", 0.09f);
    // ourShader.setFloat("spotLight.quadratic", 0.032f);
    // ourShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
    // ourShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

    ourModel.Draw(ourShader);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
    float currentX = (float)(xpos);
    float currentY = (float)(ypos);

    if (firstMouse) {
        lastX = currentX;
        lastY = currentY;
        firstMouse = false;
    }

    float xoffset = currentX - lastX;
    float yoffset = lastY - currentY;

    lastX = currentX;
    lastY = currentY;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
