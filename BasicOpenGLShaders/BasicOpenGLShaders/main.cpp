#include <iostream>

// We are using the glew32s.lib
// Thus we have a define statement
// If we do not want to use the static library, we can include glew32.lib in the project properties
// If we do use the non static glew32.lib, we need to include the glew32.dll in the solution folder
// The glew32.dll can be found here $(SolutionDir)\..\External Libraries\GLEW\bin\Release\Win32
#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include "Shader.h"

// Variables for the Height and width of the window
const GLint WIDTH = 800, HEIGHT = 600;

// SHADERS
// Shader is a type of computer program that is created by the user.
// These programs perform a variety of specialized functions in various fields of graphics special effects
// or post processing

int main()
{
	//Initializes the glfw
	glfwInit();

	// Setting the required options for GLFW

	// Setting the OpenGL version, in this case 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Setting the Profile for the OpenGL.
	// Option of choosing between Compat and Core.
	// We choose core, because we won't be using any deprecated function from the previous versions of OpenGL
	// In other words, no backward compatibility
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Setting the forward compatibility of the application to true
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// We don't want the window to resize as of now.
	// Therefore we will set the resizeable window hint to false.
	// To make is resizeable change the value to GL_TRUE.
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create the window object
	// The first and second parameters passed in are WIDTH and HEIGHT of the window we want to create
	// The third parameter is the title of the window we want to create

	// NOTE: Fourth paramter is called monitor of type GLFWmonitor, used for the fullscreen mode.
	//		 Fifth paramter is called share of type GLFWwindow, here we can use the context of another window to create this window
	// Since we won't be using any of these two features for the current tutorial we will pass nullptr in those fields
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Shaders Tutorial", nullptr, nullptr);

	// We call the function glfwGetFramebufferSize to query the actual size of the window and store it in the variables.
	// This is useful for the high density screens and getting the window size when the window has resized.
	// Therefore we will be using these variables when creating the viewport for the window
	int screenWidth, screenHeight;
	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	// Check if the window creation was successful by checking if the window object is a null pointer or not
	if (window == nullptr)
	{
		// If the window returns a null pointer, meaning the window creation was not successful
		// we print out the messsage and terminate the glfw using glfwTerminate()
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		// Since the application was not able to create a window we exit the program returning EXIT_FAILURE
		return EXIT_FAILURE;
	}

	// Creating a window does not make it the current context in the windows.
	// As a results if the window is not made the current context we wouldn't be able the perform the operations we want on it
	// So we make the created window to current context using the function glfwMakeContextCurrent() and passing in the Window object
	glfwMakeContextCurrent(window);

	// Enable GLEW, setting glewExperimental to true.
	// This allows GLEW take the modern approach to retrive function pointers and extensions
	glewExperimental = GL_TRUE;

	// Initialize GLEW to setup OpenGL function pointers
	if (GLEW_OK != glewInit())
	{
		// If the initalization is not successful, print out the message and exit the program with return value EXIT_FAILURE
		std::cout << "Failed to initialize GLEW" << std::endl;

		return EXIT_FAILURE;
	}

	// Setting up the viewport
	// First the parameters are used to set the top left coordinates
	// The next two parameters specify the height and the width of the viewport.
	// We use the variables screenWidth and screenHeight, in which we stored the value of width and height of the window,
	// using the function glfwGetFramebufferSize above.
	glViewport(0, 0, screenWidth, screenHeight);

	Shader ourShader("core.vs", "core.frag");

	// The vertices of the triangle we want to display on the screen
	GLfloat vertices[] =
	{
		// POSITION				// COLOR
		-0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f,
		0.5, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,
		0.0f, 0.5f, 0.0f,		0.0f, 0.0f, 1.0f
	};

	// VERTEX ARRAY OBJECTS (VAO)
	// The vertex array object is a special type of object that encapsulates all the data that is associated
	// with the vertex processors. Instead of containing the actual data, it holds the references to the vertex
	// buffers, the index buffers and the layout specification itself (vertices, color, normals, textures etc.)
	// The advantage of the VAO is once you setup the VAO for the mesh you can bring in the entire mesh state by
	// simply binding the VAO. After that you can render the mesh object and you don't need to worry about
	// all of its state.
	// For more information on VAO please visit this site:
	// http://ogldev.atspace.co.uk/www/tutorial32/tutorial32.html

	// VERTEX BUFFER OBJECTS (VBO)
	// Vertex buffer object is a memory buffer in the high speed memory of the video card designed to hold information
	// about the vertex.
	// Vertex buffer object creates buffer objects for vertex attributes in high performance memory on the server side
	// and provides same access functions to reference the arrays, which are used in the vertex arrays, such as
	// glVertexPointer(), glNormalPointer(), glTexCoordPointer(), etc.

	// Create variables for Vertex Buffer Objects (VBO) and Vertex Array Objects (VAO)
	GLuint VBO, VAO;

	// Generate the vertex array object names by calling the function glGenVertexArrays function.
	// The first parameter specifies the number of vertex array object names to generate
	// The second parameter specifies an array in which generated vertex array object names are stored (in our case VAO)
	glGenVertexArrays(1, &VAO);
	// Generate the vertex buffer object names by calling the function glGenBuffers function.
	// The first parameter specifies the number of vertex buffer object names to generate
	// The second parameter specifies an array in which generated vertex buffer object names are stores (in our case VBO)
	glGenBuffers(1, &VBO);

	// Activate the vertex array created (in our case VAO) active, creating it if necessary
	glBindVertexArray(VAO);

	// Activate the vertex buffer created (in our case VBO) active, creating it if necessary
	// Kind of like:
	// if (opengl->buffers[buffer] == null)
	//	   opengl->buffers[buffer] = new Buffer()
	// opengl->current_array_buffer = opengl->buffers[buffer]
	// The first parameter is the target to which the buffer is bound
	// The list of the targets can be found on this site:
	// https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBindBuffer.xhtml
	// The second parameter is name of the buffer object we want to bind to (in our case VBO)
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// The glBufferData creates a new data store for the object buffer
	// The first parameter is the target to which the buffer object is bound.
	// The list of the parameter can be found on this site:
	// https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBufferData.xhtml
	// The second parameter is the size of the buffer object data in bytes.
	// In our case the data is vertices, so we pass in sizeof(vertices) for the size of the data
	// The third parameter is the pointer to the data that will be stored in the data store.
	// The last parameter specifies the usage of the data stored in the buffer.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// Define an array of generic vertex attribute data.
	// The function specifies the format and the source buffer of a vertex attribute that is used when rendering something
	// The first parameter of the function is the index which specifies the index of the generic vertex attribute to be modified
	// The index location is specified in the vertex shader.
	// The second parameter specifies the number of components per generic vertex attribute,
	// in our case the vertex component has 3 co-ordinates value, defining the position of the vertex.
	// The thrid parameter specifies the type of each component in the array, in our case GL_FLOAT
	// The next parameter specifies whether the data values are normalized or not. If the value of this parameter is GL_TRUE,
	// it indicates the values are stored in integer format and mapped to the range [-1, 1] for signed value or [0, 1] (for unsigned value)
	// The fourth parameter is the stride which specifies the byte offset between consecutive generic vertex attribute (position).
	// The last parameter specifies the offset of the first component of the generic vertex attribute in the array data store
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	// Enable the generic vertex attribute array created above using the the function glEnableVertexAttributeArray,
	// passing in the index of the vertex attribute array
	glEnableVertexAttribArray(0);

	// Define an array of generic vertex attribute data.
	// The function specifies the format and the source buffer of a vertex attribute that is used when rendering something
	// The first parameter of the function is the index which specifies the index of the generic vertex attribute to be modified
	// The index location is specified in the vertex shader.
	// The second parameter specifies the number of components per generic vertex attribute,
	// in our case the vertex component has 3 co-ordinates value, defining the color of the vertex.
	// The thrid parameter specifies the type of each component in the array, in our case GL_FLOAT
	// The next parameter specifies whether the data values are normalized or not. If the value of this parameter is GL_TRUE,
	// it indicates the values are stored in integer format and mapped to the range [-1, 1] for signed value or [0, 1] (for unsigned value)
	// The fourth parameter is the stride which specifies the byte offset between consecutive generic vertex attribute (position).
	// The last parameter specifies the offset of the first component of the generic vertex attribute in the array data store
	// In our case the first instance of color attribute appears after 3 co-ordinate values which define the position.
	// Thus we would require to skip the 3 float values. So, we set the offset accordingly
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	// Enable the generic vertex attribute array created above using the the function glEnableVertexAttributeArray,
	// passing in the index of the vertex attribute array
	glEnableVertexAttribArray(1);

	// Unbind the buffer previously bound by passing in 0 to the glBindBuffer function.
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Unbind the existing vertex array object binding by passing in 0 to the glBindVertexArray function.
	// NOTE: we unbind the vertex array here for later use
	//		 as soon as we want to draw an object, we simply bind the VAO with preferred settings before drawing the object
	glBindVertexArray(0);

	// This is the game loop, the game logic and render part goes in here.
	// It checks if the created window is still open, and keeps performing the specified operations until the window is closed
	while (!glfwWindowShouldClose(window))
	{
		// Checking for events/inputs
		glfwPollEvents();

		// handle game object

		// render here

		// Specifies the RGBA values which will be used by glClear to clear the color buffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

		// Clears contents of a screen to a preset value, previously selected, by passing in what buffer we want to clear
		// In this case we are clearing the color buffer. Thus setting a background color to the color specified previously in glClearColor
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw OpenGL stuff
		// Use the current shader
		ourShader.Use();
		// Bind the VAO here for the purpose of drawing using the settings required
		glBindVertexArray(VAO);
		// Draw the primitive shapes from the vertex array data.
		// The primitive datas can be points, lines, triangles etc.
		// Here is an example of all the primitives in OPENGL
		// https://www.khronos.org/opengl/wiki/Primitive
		glDrawArrays(GL_TRIANGLES, 0, 3);
		// Unbind the vertex array here, so that we can bind a different VAO.
		// NOTE: Since we are only using a single VAO here, it is not necessary to unbind it here, but we do it for completeness sake.
		glBindVertexArray(0);

		// Swaps the front and back buffers of the specified window
		glfwSwapBuffers(window);
	}

	// Delete the vertex array object, passing in the number of the vertex arrays objects stored in the the array (VAO)
	glDeleteVertexArrays(1, &VAO);
	// Delete the number of buffer objects passed in the array buffer.
	glDeleteBuffers(1, &VBO);

	// Terminate all the stuff related to GLFW and exit the program using the return value EXIT_SUCCESS
	glfwTerminate();

	return EXIT_SUCCESS;
}