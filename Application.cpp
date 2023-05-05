#include "App.h"

#include "Constants.h"

#include "stb_image.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Shader.h"
#include "Camera.h"
#include "Model.h"

#include "shader_m.h"
#include "Camera.h"
#include "animator.h"
#include "model_animation.h"
#include "Picking_Texture.h"

#include "BasicCube.h"

#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void processInput(GLFWwindow* window);
unsigned int loadTexture(char const* path);


void launchItem();

glm::vec3 lightPos(-3.0f, 0.0f, 0.0f);

int changeVar = 0;
int currentVar = 0;
bool isChanged = false;

bool checkBoxCollide(BasicCube a, BasicCube b);

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 8.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

Animation currentAnimation;


vector<glm::vec3> positions;

vector<BasicCube> launchObjects;

int oldState = GLFW_RELEASE;

bool extend = false;

float xDir = 0.0f;
float yDir = 0.0f;



glm::vec3 cubePositions[] = {
        glm::vec3(-2.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
};

float x = 0.0f;



int main()
{
    App app;
    app.init();

    glfwMakeContextCurrent(app.window);
    glfwSetFramebufferSizeCallback(app.window, framebuffer_size_callback);
    glfwSetScrollCallback(app.window, scroll_callback);
    glfwSetCursorPosCallback(app.window, mouse_callback);
    glfwSetMouseButtonCallback(app.window, mouse_button_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    Shader lightCubeShader("shaders/third.vert", "shaders/lightCube.frag");
    Shader lightingShader("shaders/lighting.vert", "shaders/multipleLights.frag");
    Shader modelShader("shaders/modelShader.vert","shaders/modelShader.frag");
    Shader animShader("animShader.vert","animShader.frag");

    Model ourModel("resources/test/Untitled.obj");

    AnimModel ourModelnew("resources/walk/walk.dae");
    Animation danceAnimation("resources/dancing/dancing.dae", &ourModelnew);
    Animation walkAnimation("resources/walk/walk.dae",&ourModelnew);

    
    if(currentVar==0)
        currentAnimation = walkAnimation;

    Animator animator(&currentAnimation);
    
    vector<BasicCube> bc(10);

    vector<BasicCube> body(2);

    BasicCube lightCube;

    positions.push_back(glm::vec3(xDir, yDir, 0.0f));
    positions.push_back(glm::vec3(xDir, yDir, -15.0f));

    for (int i = 0; i < body.size(); i++)
    {
        body[i].position = positions[i];
    }

    for (int i=0;i<10;i++)
    {
        bc[i].position = cubePositions[i];
    }


    unsigned int diffuseMap = loadTexture("resources/container2.png");
    unsigned int specularMap = loadTexture("resources/container2_specular.png");

    BasicCube cubeone;
    BasicCube cubetwo;

    unsigned int texture1, texture2;
    // texture 1
    // ---------
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // note that we set the container wrapping method to GL_CLAMP_TO_EDGE
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    unsigned char* data = stbi_load("resources/wall.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    // texture 2
    // ---------
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// we want to repeat the awesomeface pattern so we kept it at GL_REPEAT
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // load image, create texture and generate mipmaps
    data = stbi_load("resources/awesomeface.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        // note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);



    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glEnable(GL_DEPTH_TEST);

    // positions of the point lights
    glm::vec3 pointLightPositions[] = {
        glm::vec3(0.7f,  0.2f,  2.0f),
        glm::vec3(2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3(0.0f,  0.0f, -3.0f)
    };


    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(app.window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    float lightX = 0.0f, lightY = 0.0f, lightZ = 0.0f;
    lightingShader.use();
    lightingShader.setInt("material.diffuse", 0);
    lightingShader.setInt("material.specular", 1);

    float xChange = 0.0f;

    float animationBlendFactor = 0.0f;

    DirectionalLight dl;
    vector<PointLight> pl(4);
    SpotLight sp;

    lightCube.setPosition(lightPos);
    cubetwo.setPosition(glm::vec3(-4.0f, 0.0f, 0.0f));


    lightCube.enablePhysics();
    cubeone.enablePhysics();
    cubetwo.enablePhysics();

    while (!glfwWindowShouldClose(app.window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(app.window);

        if (extend == true)
        {
            extend = false;
            std::cout << "Shit Works" << std::endl;
            BasicCube temp;

            positions.push_back(glm::vec3(positions[positions.size()-1].x+1.0f,0.0f,0.0f));
            
            temp.position = positions[positions.size() - 1];
            body.push_back(temp);
        }

        for (int i = 0; i < body.size(); i++)
        {
            positions[i] = glm::vec3(x + static_cast<float>(i), 0.0f, 0.0f);
        }

        //std::cout << checkBoxCollide(bc[0], body[0]) << std::endl;

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!


        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Test Window");
        ImGui::Text("Hello W0rld");
        ImGui::SliderFloat("animationBlendFactor", &animationBlendFactor, 0.0f, 1.0f);
        ImGui::SliderFloat("LightY", &lightY, 0.0f, 2.0f);
        ImGui::SliderFloat("LightX", &lightX, 0.0f, 2.0f);
        ImGui::SliderFloat("xChange", &xChange, -4.0f, 4.0f);
        ImGui::End();


        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);



        // camera/view transformation
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        view = camera.GetViewMatrix();



        float radius = 10.0f;


        // create transformations
        //glm::mat4 view = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        
        // Code for locking on to an object
        /*float x = cubePositions[0].x;
        float y = cubePositions[0].y;
        float z = cubePositions[0].z;


        camera.Position = glm::vec3(x,y,z+3.0f);
        camera.updateCameraVectors();*/

        

        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();
        lightingShader.setVec3("viewPos", camera.Position);
        lightingShader.setFloat("material.shininess", 32.0f);

        /*
           Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index
           the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
           by defining light types as classes and set their values in there, or by using a more efficient uniform approach
           by using 'Uniform buffer objects', but that is something we'll discuss in the 'Advanced GLSL' tutorial.
        */


        // directional light


        dl.lightUp(
            lightingShader,
            glm::vec3(-0.2f, -1.0f, -0.3f),     //direction
            glm::vec3(lightY, lightZ, 0.08f),       //ambient
            glm::vec3(0.6f, 0.6f, 0.6f),        //diffuse
            glm::vec3(0.5f, 0.5f, 0.5f)         //specular
        );
        

        pl[0].lightUp(lightingShader,
            pointLightPositions[0],
            glm::vec3(lightX, 0.05f, 0.05f),
            glm::vec3(0.8f, 0.8f, 0.8f),
            glm::vec3(1.0f, 1.0f, 1.0f),
            1.0f,
            0.09f,
            0.032f,
            0
        );
    
        pl[1].lightUp(lightingShader,
            pointLightPositions[1],
            glm::vec3(lightX, 0.05f, 0.05f),
            glm::vec3(0.8f, 0.8f, 0.8f),
            glm::vec3(1.0f, 1.0f, 1.0f),
            1.0f,
            0.09f,
            0.032f,
            1
        );

        pl[2].lightUp(lightingShader,
            pointLightPositions[2],
            glm::vec3(lightX, 0.05f, 0.05f),
            glm::vec3(0.8f, 0.8f, 0.8f),
            glm::vec3(1.0f, 1.0f, 1.0f),
            1.0f,
            0.09f,
            0.032f,
            2
        );

        pl[3].lightUp(lightingShader,
            pointLightPositions[3],
            glm::vec3(0.05f, 0.05f, 0.05f),
            glm::vec3(0.8f, 0.8f, 0.8f),
            glm::vec3(1.0f, 1.0f, 1.0f),
            1.0f,
            0.09f,
            0.032f,
            3
        );


        sp.lightUp(
            lightingShader,
            camera.Position,
            camera.Front,
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(1.0f, 1.0f, 1.0f),
            glm::vec3(1.0f, 1.0f, 1.0f),
            1.0f,
            0.09f,
            0.032f,
            glm::cos(glm::radians(12.5f)),
            glm::cos(glm::radians(15.0f))
        );
        

        // view/projection transformations
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);


        // bind diffuse map
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        // bind specular map
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        //model = glm::translate(model, glm::vec3(-4.0f, 0.0f, 0.0f));
        
        cubetwo.Draw(lightingShader, deltaTime);


        // pass transformation matrices to the shader
        lightingShader.setMat4("projection", projection); // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        lightingShader.setMat4("view", view);


        for (unsigned int i = 0; i < 10; i++)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f);
            //model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            if (i % 3 == 0)  // every 3rd iteration (including the first) we set the angle using GLFW's time function.
                angle = glfwGetTime() * 25.0f;

            //if (!(i == 0 || i == 1))
                //model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

            bc[i].setPosition(cubePositions[i]);
            bc[i].setRotation(glm::vec3(1.0f, 0.3f, 0.5f), angle);
            bc[i].Draw(lightingShader, deltaTime);
        }



        for (unsigned int i = 0; i < body.size(); i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            //model = glm::translate(model, positions[i]);
            body[i].setPosition(positions[i]);
            body[i].Draw(lightingShader, deltaTime);
        }



        lightCubeShader.use();
        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
        

        lightCube.setScale(glm::vec3(0.2));
        lightCube.Draw(lightCubeShader, deltaTime);
        
        cubeone.Draw(lightCubeShader, deltaTime);


        for (int i = 0; i < launchObjects.size(); i++)
        {
            launchObjects[i].Draw(lightCubeShader, deltaTime);
        }


        modelShader.use();

        // view/projection transformations
        modelShader.setMat4("projection", projection);
        modelShader.setMat4("view", view);

        // render the loaded model
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(4.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));	// it's a bit too big for our scene, so scale it down
        //modelShader.setMat4("model", model);
        ourModel.Draw(modelShader, glm::vec3(4.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f), 0.0f, glm::vec3(1.0f));


        if (currentVar != changeVar)
        {
            currentVar = changeVar;
            if (currentVar == 0)
            {
                currentAnimation = walkAnimation;
            }
            else if (currentVar == 1)
            {
                currentAnimation = danceAnimation;
            }
            //Animator animator(&currentAnimation);
        }


        //animator.UpdateAnimation(deltaTime);
        animator.BlendTwoAnimations(&walkAnimation, &danceAnimation, animationBlendFactor, deltaTime); // 30.0f intentional here, otherwise they play too slowly
        
        
        animShader.use();
        animShader.setMat4("projection", projection);
        animShader.setMat4("view", view);

        auto transforms = animator.GetFinalBoneMatrices();
        for (int i = 0; i < transforms.size(); ++i)
            animShader.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);

        // render the loaded model
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -0.4f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(.005f, .005f, .005f));	// it's a bit too big for our scene, so scale it down
        animShader.setMat4("model", model);
        ourModelnew.Draw(animShader);



        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(app.window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    //glDeleteVertexArrays(2, VAOs);
    //glDeleteBuffers(2, VBOs);
    //glDeleteProgram(shaderProgramOrange);
    //glDeleteProgram(shaderProgramYellow);

    glfwTerminate();

    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        x += 0.01f;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        x -= 0.01f;
    }

    

    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && extend==false && oldState==GLFW_RELEASE)
    {
        extend = true;
        oldState = GLFW_PRESS;
        launchItem();
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_RELEASE && oldState == GLFW_PRESS)
    {
        oldState = GLFW_RELEASE;
    }

    

    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS && isChanged==false)
    {
        if (changeVar == 0)
            changeVar = 1;
        else if (changeVar == 1)
            changeVar = 0;

        isChanged = true;
    }
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_RELEASE && isChanged == true)
    {
        isChanged = false;
    }

    const float cameraSpeed = 0.05f; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);


    

    
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        float xpos = static_cast<float>(xposIn);
        float ypos = static_cast<float>(yposIn);

        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

        lastX = xpos;
        lastY = ypos;

        camera.ProcessMouseMovement(xoffset, yoffset);
    }
    
    
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        double xpos, ypos;
        //getting cursor position
        glfwGetCursorPos(window, &xpos, &ypos);
        
    }
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
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

void launchItem()
{
    BasicCube newCube;

    newCube.setPosition(camera.Position);
    newCube.enablePhysics();

    newCube.rb.mass = 1.0f;

    newCube.rb.acceleration = Environment::gravitationalAcceleration;
    newCube.rb.applyImpulse(camera.Front, 1000.0f, deltaTime);

    launchObjects.push_back(newCube);
}

bool checkBoxCollide(BasicCube a, BasicCube b)
{
    glm::vec3 aPosMin = glm::vec3(a.position.x, a.position.y - 1.0f, a.position.z);
    glm::vec3 aPosMax = glm::vec3(a.position.x + 1.0f, a.position.y, a.position.z - 1.0f);


    glm::vec3 bPosMin = glm::vec3(b.position.x, b.position.y - 1.0f, b.position.z);
    glm::vec3 bPosMax = glm::vec3(b.position.x + 1.0f, b.position.y, b.position.z - 1.0f);


    std::cout << "aX" << aPosMin.x << " : " << aPosMax.x << endl;
    std::cout << "aY" << aPosMin.y << " : " << aPosMax.y << endl;
    std::cout << "aZ " << aPosMin.z << " : " << aPosMax.z << endl;

    std::cout << "bX" << bPosMin.x << " : " << bPosMax.x << endl;
    std::cout << "bY" << bPosMin.y << " : " << bPosMax.y << endl;
    std::cout << "bX"  << bPosMin.z << " : " << bPosMax.z << endl;

    std::cout<< (aPosMin.x <= bPosMax.x) << endl;
    std::cout << "error" << (aPosMax.x >= bPosMin.x) << endl;
    std::cout << aPosMax.x << ">=" << bPosMin.x << endl;
    std::cout << (aPosMin.y <= bPosMax.y) << endl;
    std::cout << (aPosMax.y >= bPosMin.y) << endl;
    std::cout << (aPosMin.z <= bPosMax.z) << endl;
    std::cout << (aPosMax.z >= bPosMin.z) << endl;

    return (
        aPosMin.x <= bPosMax.x &&
        aPosMax.x >= bPosMin.x && //
        aPosMin.y <= bPosMax.y &&
        aPosMax.y >= bPosMin.y &&
        aPosMin.z <= bPosMax.z &&
        aPosMax.z >= bPosMin.z
        );
}