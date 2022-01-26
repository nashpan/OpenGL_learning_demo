/*
***  reference:https://antongerdelan.net/opengl/compute.html
***  instruction: create a texture and use compute shader to deal with the texture,then render the texture as normal!
***
*/


#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(500, 500, "Title", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    gl3wInit();
    const GLubyte* version = glGetString(GL_VERSION);
       
    const char* ray_shader_string = R"(
    #version 430
    layout(local_size_x = 1, local_size_y = 1) in;
    layout(rgba32f, binding = 0) uniform image2D img_output;

    void main() {
      // base pixel colour for image
        vec4 pixel = vec4(0.0, 0.0, 0.0, 1.0);
      // get index in global work group i.e x,y position
        ivec2 pixel_coords = ivec2(gl_GlobalInvocationID.xy);
  
        float max_x = 5.0;
        float max_y = 5.0;
        ivec2 dims = imageSize(img_output); // fetch image dimensions
        float x = (float(pixel_coords.x * 2 - dims.x) / dims.x);
        float y = (float(pixel_coords.y * 2 - dims.y) / dims.y);
        vec3 ray_o = vec3(x * max_x, y * max_y, 0.0);
        vec3 ray_d = vec3(0.0, 0.0, -1.0); // ortho
  
        vec3 sphere_c = vec3(0.0, 0.0, -10.0);
        float sphere_r = 1.0;
        
        vec3 omc = ray_o - sphere_c;
        float b = dot(ray_d, omc);
        float c = dot(omc, omc) - sphere_r * sphere_r;
        float bsqmc = b * b - c;
        // hit one or both sides
        if (bsqmc >= 0.0) {
          pixel = vec4(0.4, 0.4, 1.0, 1.0);
        }
        
      // output to a specific pixel in the image
        imageStore(img_output, pixel_coords, pixel);
    }
    )";

    GLuint ray_shader = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(ray_shader, 1, &ray_shader_string, NULL);
    glCompileShader(ray_shader);
    int success;
    char infoLog[512];
    glGetShaderiv(ray_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(ray_shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    GLuint ray_program = glCreateProgram();
    glAttachShader(ray_program, ray_shader);
    glLinkProgram(ray_program);

    // normal shader
    const char* vertexShaderSource = R"(
        #version 430 core
        layout (location = 0) in vec2 aPos;
        layout (location = 1) in vec2 in_uv;
        out vec2 out_uv;
        void main()
        {
            gl_Position = vec4(aPos.x, aPos.y, 0, 1.0);
            out_uv = in_uv;
        })";

    const char* fragmentShaderSource = R"(
        #version 430 core
        out vec4 FragColor;
        in vec2 out_uv;
        uniform sampler2D texture1;
        void main()
        {
            FragColor = texture(texture1, out_uv);
        })";

    float vertices[] = {
        // positions             // texture coords
          1.0f,  1.0f, 1.0f, 1.0f, // top right
          1.0f, -1.0f, 1.0f, 0.0f, // bottom right
         -1.0f, -1.0f, 0.0f, 0.0f, // bottom left
         -1.0f,  1.0f, 0.0f, 1.0f  // top left 
    };

    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
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

    GLuint VAO, VBO, EBO;;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    int tex_w = 512, tex_h = 512;
    GLuint tex_output;
    glGenTextures(1, &tex_output);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex_output);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);   
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, tex_w, tex_h, 0, GL_RGBA, GL_FLOAT, NULL);
    glBindImageTexture(0, tex_output, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

    glUseProgram(shaderProgram);
    GLint location = glGetUniformLocation(shaderProgram, "texture1");
    glUniform1i(location, 0);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(ray_program);
        glDispatchCompute((GLuint)tex_w, (GLuint)tex_h, 1);

        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex_output);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();

    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
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
