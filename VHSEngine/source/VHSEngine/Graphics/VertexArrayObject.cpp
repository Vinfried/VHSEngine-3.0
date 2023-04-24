#include "VHSEngine/Graphics/VertexArrayObject.h"
#include "GL/glew.h"

VertexArrayObject::VertexArrayObject(GeometricShapes ChosenShape)
{
	ID = EAB = VAB = 0;

	//localised version of matrices
	PositionMatrix ChosenPositions = PositionMatrix();
	IndicesMatrix ChosenIndices = IndicesMatrix();

	//switch the chosen matrices depending on the selected geometry
	switch (ChosenShape) {
	case GeometricShapes::Triangle:
		ChosenPositions = TrianglePositions;
		ChosenIndices = TriangleIndices;
		break;
	case GeometricShapes::Polygon:
		ChosenPositions = PolyPositions;
		ChosenIndices = PolyIndices;
		break;
	case GeometricShapes::Circle:
		ChosenPositions = CirclePositions;
		ChosenIndices = CircleIndices;
		break;
	case GeometricShapes::Cube:
		ChosenPositions = CubePositions;
		ChosenIndices = CubeIndices;
		break;
	default:
		break;
	}

	Shape.PositionMatrix = ChosenPositions;
	Shape.IndicesMatrix = ChosenIndices;


	//Handle the positons
	//Create the ID for our VAO
	glGenVertexArrays(1, &ID);

	//bind the data to this vertex array
	glBindVertexArray(ID);

	//create an id for our array buffer
	glGenBuffers(1, &VAB);
	//bind the above IDs to the OpenGL
	glBindBuffer(GL_ARRAY_BUFFER, VAB);
	//run through the data and attach the vertices to VAB
	glBufferData(GL_ARRAY_BUFFER,
		Shape.PositionMatrix.size() * sizeof(float),
		&Shape.PositionMatrix[0],
		GL_STATIC_DRAW);

	//Handle the indices
	//Create an id for our element array buffer
	glGenBuffers(1, &EAB);
	// bind the above ID to openGL as the element buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EAB);
	//run through the data and attach the indices to the EAB
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		Shape.IndicesMatrix.size() * sizeof(vhsuint),
		&Shape.IndicesMatrix[0],
		GL_STATIC_DRAW);

	//assign the vertices and indices to VAO
	glVertexAttribPointer(
		0,					//Data Set - 0 = the first data set in the array
		3,					//How many numbers in our matrix to make a triangle
		GL_FLOAT, GL_FALSE,	//data type, whether you want to normalise the values
		sizeof(float) * 8,	//stride - the length it takes to get to each number
		(void*)0			//offset of how many numbers to skip the matrix
	);

	//enable the vertex array
	glEnableVertexAttribArray(0);

	//Assign the colour to the sender
	glVertexAttribPointer(
		1,
		3,
		GL_FLOAT, GL_FALSE,
		sizeof(float) * 8,
		(void*)(3 * sizeof(float))
	);

	//enabling the colour array
	glEnableVertexAttribArray(1);

	//Assign the texture coordinates to the shader
	glVertexAttribPointer(
		2,
		2,
		GL_FLOAT, GL_FALSE,
		sizeof(float) * 8,
		(void*)(6 * sizeof(float))
	);

	//enabling the texture coordinate array
	glEnableVertexAttribArray(2);

	//clear buffer
	glBindVertexArray(0);
}

VertexArrayObject::VertexArrayObject(vector<Vertex> Vertices, vector<vhsuint> Indices)
{
	ID = EAB = VAB = 0;

	//assign the verticies and indices to the class
	this->Vertices = Vertices;
	this->Indices = Indices;
	
	//Create the ID for our VAO
	glGenVertexArrays(1, &ID);

	//bind the data to this vertex array
	glBindVertexArray(ID);

	//create an id for our array buffer
	glGenBuffers(1, &VAB);
	//bind the above IDs to the OpenGL
	glBindBuffer(GL_ARRAY_BUFFER, VAB);
	//run through the data and attach the vertices to VAB
	glBufferData(
		GL_ARRAY_BUFFER,
		Vertices.size() * sizeof(Vertex),
		&Vertices[0],
		GL_STATIC_DRAW);

	//Handle the indices
	//Create an id for our element array buffer
	glGenBuffers(1, &EAB);
	// bind the above ID to openGL as the element buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EAB);
	//run through the data and attach the indices to the EAB
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		Indices.size() * sizeof(vhsuint),
		&Indices[0],
		GL_STATIC_DRAW);

	//assign the vertices and indices to VAO
	glVertexAttribPointer(
		0,					//Data Set - 0 = the first data set in the array
		3,					//How many numbers in our matrix to make a triangle
		GL_FLOAT, GL_FALSE,	//data type, whether you want to normalise the values
		sizeof(float) * 8,	//stride - the length it takes to get to each number
		(void*)0			//offset of how many numbers to skip the matrix
	);

	//enable the vertex array
	glEnableVertexAttribArray(0);

	//Assign the normals of the mesh vertices to the sender
	glVertexAttribPointer(
		1,
		3,
		GL_FLOAT, GL_FALSE,
		sizeof(float) * 8,
		(void*)(3 * sizeof(float))
	);

	//enabling the colour array
	glEnableVertexAttribArray(1);

	//Assign the texture coordinates to the shader
	glVertexAttribPointer(
		2,
		2,
		GL_FLOAT, GL_FALSE,
		sizeof(float) * 8,
		(void*)(6 * sizeof(float))
	);

	//enabling the texture coordinate array
	glEnableVertexAttribArray(2);

	//clear buffer
	glBindVertexArray(0);

}

VertexArrayObject::~VertexArrayObject()
{
	//clean tup the VAO in openGL
	glDeleteVertexArrays(1, &ID);

	//Clean up the vectors
	Shape.PositionMatrix.clear();
	Shape.IndicesMatrix.clear();
	Vertices.clear();
	Indices.clear();

	cout << "Delete VAO..." << endl;
}

void VertexArrayObject::Draw()
{
	vector<vhsuint> IndicesToUse;

	//decide which indices to use based on whats assigned
	if (Vertices.size() > 0)
		IndicesToUse = Indices;
	else
		IndicesToUse = Shape.IndicesMatrix;



	//bind our VAO to the current buffer
	glBindVertexArray(ID);
	//Draw the 3d object/VAO
	glDrawElements(
		GL_TRIANGLES,					//what type of objects are we drawing
		IndicesToUse.size(),		//how many vertices do we draw
		GL_UNSIGNED_INT,				//what is the type of data that's being input
		(void*)0						//how many should we skip
	);
	//clear the VAO from the current array to allow for the nect object
	glBindVertexArray(0);
}
