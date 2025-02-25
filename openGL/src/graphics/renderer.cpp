#include "renderer.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const *path)
{
  unsigned int textureID;
  glGenTextures(1, &textureID);

  int width, height, nrComponents;
  unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
  if (data)
  {
    GLenum format;
    if (nrComponents == 1)
      format = GL_RED;
    else if (nrComponents == 3)
      format = GL_RGB;
    else if (nrComponents == 4)
      format = GL_RGBA;

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
  }
  else
  {
    std::cout << "Texture failed to load at path: " << path << std::endl;
    stbi_image_free(data);
  }

  return textureID;
}

/**
  @brief constructor for Renderer class. Initializes shaders, VAO, and VBO, and loads textures.
*/
Renderer::
    Renderer()
{
  blockShader = new Shader("shaders/blockVertexShader.glsl", "shaders/blockFragShader.glsl");

  // load textures
  diffuseMap = loadTexture("textures/container2.png");
  specularMap = loadTexture("textures/container2_specular.png");


}

/**
  @brief destructor for Renderer class. Cleans up resources.
*/
Renderer::~Renderer()
{
  delete blockShader;
}

/**
  @brief clears the screen
*/
void Renderer::clear()
{
  // clear the colorbuffer
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/**
  @brief draws the scene
*/
void Renderer::draw(Player *player, Window *window, World *world)
{
  
  // render the triangle

  // send uniforms to shader
  blockShader->use();

  glm::mat4 model = glm::mat4(1.0f);
  glm::mat4 view;
  view = player->getCameraView();
  glm::mat4 projection;
  projection = glm::perspective(glm::radians(55.0f), (float)window->width / (float)window->height, 0.1f, 100.0f);

  // send uniforms to shader
  blockShader->setMat4("view", view);
  blockShader->setMat4("projection", projection);
  blockShader->setInt("diffuse", 0);

  // bind textures
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, diffuseMap);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, specularMap);

  world->render(blockShader);

  glBindVertexArray(0);

  glfwPollEvents();
}