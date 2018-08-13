#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>

class Shader
{
public:
	GLuint shaderProgram;
	
	// Constructor generates the shader on the fly
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
	{
		//Retrieve the vertex/fragment source code from filePath
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		// ensures ifstream objects can throw exceptions:
		vShaderFile.exceptions(std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::badbit);
		try
		{
			// Open files
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;
			// Read file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// close file handlers
			vShaderFile.close();
			fShaderFile.close();
			// Convert stream into string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}
		// Covert the strings into C strings
		const GLchar* vertexShaderCode = vertexCode.c_str();
		const GLchar* fragmentShaderCode = fragmentCode.c_str();
		
		//Compile shaders
		// Create variables for the shader objects
		GLuint vertexShader, fragmentShader;
		// Create two variables check the status of the compilation of the shaders
		// These variables will be reused while checking states of all different shaders and linking
		GLint success;
		GLchar infoLog[512];
		
		// Vertex Shader

		// Create an empty shader object, providing what type of shader we will be compiling
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		// Set the source code in the shader (in our case vertexShader, the object created above)
		// to the source code in the array of the strings (in our case vertexShaderSource)
		// The second parameter is count, which is the count for the number of string in the array (in our case, 1).
		// The next parameter is the reference to the actual source code string (in our case vertexShaderSource)
		// The last parameter is the length of the string of the source code. If the value of the length is NULL,
		// the program assumes that the string will end with a null character, if the value is anything other than NULL,
		// it points to an array containing a string length for each of the corresponding elements of the string.
		glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
		// Compiles the source code that has been stored in the shader object which is passed as the parameter
		glCompileShader(vertexShader);
		
		// Returns the status of the of the parameter for the specified object file
		// First parameter is the shader object which is to be queried
		// Second object is object parameter we want to check for. Some of the examples are GL_SHADER_TYPE, GL_COMPILE_STATUS etc.
		// Third parameter is the where the return value for the query has been stored
		// In our case we will be checking the parameter GL_COMPILE_STATUS, which checks whether the vertex shader compilation
		// was successful or not
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

		// Check if the compilation was successful or not
		if (!success)
		{
			// If compilation had failed,
			// get the information log using the function glGetShaderInfoLog of the required shader object
			// The first parameter is the shader object whose information log we want,
			// the second parameter is the size of the buffer for storing the info log
			// the third parameter is the length of the string of returned (if not terminated by null charater)
			// the fourth parater is the char array where the infolog will be stored
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		// Fragment Shader

		// Follow the same steps for the fragment shader as we did for the vertex shader above.
		// Changing the shader objects and parameters to point to fragment shader instead of the vertex shader
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
		glCompileShader(fragmentShader);
		// Print compile errors if any
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		// Shader Program, Linking the shaders

		// Create a program object using the glCreateProgram
		this->shaderProgram = glCreateProgram();
		// Attach the shader objects (vertexShader & fragmentShader) to the program object (shaderProgram)
		glAttachShader(this->shaderProgram, vertexShader);
		glAttachShader(this->shaderProgram, fragmentShader);
		// Links the program object
		// If any shader objects are attached to the program object, they will be used to create an executable,
		// which will be run on the respective programmable processor (vertex shader will run on the the vertex programmable processor)
		glLinkProgram(this->shaderProgram);

		// Print linking errors if any
		// Similar to gtGetShaderiv, glGetProgramiv checks whether the link of the program object was successful or not
		// and stores the result in the return value parameter (in our case success, which is again being reused)
		glGetProgramiv(this->shaderProgram, GL_LINK_STATUS, &success);

		// Check whether the linking was successful or not
		if (!success)
		{
			// Similar to glGetShaderInfoLog, glGetProgramInfoLog provides the information about the program object,
			// the parameters are the same as glGetShaderProgram passing in the program object instead of the shader object
			// infoLog variable is reused here as well
			glGetProgramInfoLog(this->shaderProgram, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}
		// Delete the shaders as they're linked into our program now and no longer necessery
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

	}
	// Uses the current shader
	void Use()
	{
		// Install the program object specified, as a part of the current rendering state. In our case shaderProgram
		// The program object has different executables previously attached to it using glAttachShader and glLinkProgram.
		// The program object will run on the respective processor depending on the type of shader objects
		// that have been compiled and linked to the program object
		// In our case the shaderProgram has a vertex shader and a fragment shader attached,
		// Meaning those will run on the vertex processor and the fragment processor respectively.
		glUseProgram(this->shaderProgram);
	}
};

#endif