#include "renderer.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

glm::vec3 pointLightPositions[] = {
	glm::vec3( 0.7f,  0.2f,  2.0f),
	glm::vec3( 2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3( 0.0f,  0.0f, -3.0f)
};  

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
  // create shaders
  lightingShader = new Shader("shaders/vertexShader.glsl", "shaders/fragmentShader.glsl");
  lightCubeShader = new Shader("shaders/vertexShader.glsl", "shaders/lightcubeFragShader.glsl");

  // create VAO and VBO
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // normal vector attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // texture attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  diffuseMap = loadTexture("textures/container2.png");
  specularMap = loadTexture("textures/container2_specular.png");
}

/**
  @brief destructor for Renderer class. Cleans up resources.
*/
Renderer::~Renderer()
{
  delete lightingShader;
  delete lightCubeShader;
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
void Renderer::draw(Player *player, Window *window)
{
  float timeValue = glfwGetTime();
  float h_offset = sin(timeValue);
  glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

  // render the triangle

  // send uniforms to shader
  lightingShader->use();

  glm::mat4 model = glm::mat4(1.0f);
  glm::mat4 view;
  view = player->getCameraView();
  glm::mat4 projection;
  projection = glm::perspective(glm::radians(55.0f), (float)window->width / (float)window->height, 0.1f, 100.0f);

  // send uniforms to shader
  lightingShader->setMat4("view", view);
  lightingShader->setMat4("projection", projection);

  lightingShader->setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));

  // make light source move
  lightPos.x = 1.0f + h_offset;
  lightPos.y = 0.0f;
  lightPos.z = cos(timeValue) - 5.0f;

  // player position
  lightingShader->setVec3("viewPos", player->playerPos);

  // material properties
  lightingShader->setInt("material.diffuse", 0);
  lightingShader->setInt("material.specular", 1);
  lightingShader->setFloat("material.shininess", 32.0f);

  // set dir light properties
  lightingShader->setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);  
  lightingShader->setVec3("dirLight.ambient", 0.2f, 0.2f, 0.2f);  
  lightingShader->setVec3("dirLight.diffuse", 0.5f, 0.5f, 0.5f);  
  lightingShader->setVec3("dirLight.specular", 1.0f, 1.0f, 1.0f);  


  // set point lightproperties POINT 0
  lightingShader->setVec3("pointLights[0].position", pointLightPositions[0]);
  lightingShader->setVec3("pointLights[0].ambient", 0.2f, 0.2f, 0.2f);
  lightingShader->setVec3("pointLights[0].diffuse", 0.5f, 0.5f, 0.5f); 
  lightingShader->setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
  lightingShader->setFloat("pointLights[0].constant", 0.5f);
  lightingShader->setFloat("pointLights[0].linear", 0.09f);
  lightingShader->setFloat("pointLights[0].quadratic", 0.032f);

  // set point lightproperties POINT 1
  lightingShader->setVec3("pointLights[1].position", pointLightPositions[1]);
  lightingShader->setVec3("pointLights[1].ambient", 0.2f, 0.2f, 0.2f);
  lightingShader->setVec3("pointLights[1].diffuse", 0.5f, 0.5f, 0.5f); 
  lightingShader->setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
  lightingShader->setFloat("pointLights[1].constant", 0.5f);
  lightingShader->setFloat("pointLights[1].linear", 0.09f);
  lightingShader->setFloat("pointLights[1].quadratic", 0.032f);

  // set point lightproperties POINT 2
  lightingShader->setVec3("pointLights[2].position", pointLightPositions[2]);
  lightingShader->setVec3("pointLights[2].ambient", 0.2f, 0.2f, 0.2f);
  lightingShader->setVec3("pointLights[2].diffuse", 0.5f, 0.5f, 0.5f); 
  lightingShader->setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
  lightingShader->setFloat("pointLights[2].constant", 0.5f);
  lightingShader->setFloat("pointLights[2].linear", 0.09f);
  lightingShader->setFloat("pointLights[2].quadratic", 0.032f);

  // set point lightproperties POINT 3
  lightingShader->setVec3("pointLights[3].position", pointLightPositions[3]);
  lightingShader->setVec3("pointLights[3].ambient", 0.2f, 0.2f, 0.2f);
  lightingShader->setVec3("pointLights[3].diffuse", 0.5f, 0.5f, 0.5f); 
  lightingShader->setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
  lightingShader->setFloat("pointLights[3].constant", 0.5f);
  lightingShader->setFloat("pointLights[3].linear", 0.09f);
  lightingShader->setFloat("pointLights[3].quadratic", 0.032f);

  // bind textures
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, diffuseMap);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, specularMap);

  // draw all the cubes
  glBindVertexArray(VAO);
  for (unsigned int i = 0; i < 10; i++)
  {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, cubePositions[i]);
    float angle = 20.0f * i;
    model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
    lightingShader->setMat4("model", model);

    glDrawArrays(GL_TRIANGLES, 0, 36);
  }


  // draw the light cubes
  for (unsigned int i = 0; i < 4; i++)
  {
    lightCubeShader->use();
    model = glm::mat4(1.0f);
    model = glm::translate(model, pointLightPositions[i]);
    model = glm::scale(model, glm::vec3(0.2f));

    // lightingShader.setMat4("model", model);
    lightCubeShader->setMat4("model", model);
    lightCubeShader->setMat4("view", view);
    lightCubeShader->setMat4("projection", projection);
    glDrawArrays(GL_TRIANGLES, 0, 36);
  }
  glBindVertexArray(0);

  glfwPollEvents();
}