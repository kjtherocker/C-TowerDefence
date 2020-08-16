#include "GamePCH.h"

Mesh::Mesh()
{
    m_VBO = 0;
}

Mesh::~Mesh()
{
    glDeleteBuffers( 1, &m_VBO );
}

void Mesh::GenerateSprite(float width, float height)
{
    CheckForGLErrors();

    // ATM this can only be called once, so we assert if it's called again with a VBO already allocated.
    assert( m_VBO == 0 );

    // Vertex position info for a quad, pivot point is at bottom center.
    VertexFormat vertexattributes[] =
    {
        -width/2,  height,   0.0f, 1.0f, // TL
         width/2,  height,   1.0f, 1.0f, // TR
         width/2,    0.0f,   1.0f, 0.0f, // BR
        -width/2,    0.0f,   0.0f, 0.0f, // BL
    };

    // Gen and fill buffer with our attributes.
    glGenBuffers( 1, &m_VBO );
    glBindBuffer( GL_ARRAY_BUFFER, m_VBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof(VertexFormat) * 4, vertexattributes, GL_STATIC_DRAW );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );

    m_PrimitiveType = GL_TRIANGLE_FAN;
    m_NumVerts = 4;

    CheckForGLErrors();
}

void Mesh::Draw(ShaderProgram* pShader, GLuint texture, vec2 pos,vec2 UVoffset, vec2 UVscale, Camera * cam)
{
    CheckForGLErrors();

    // Use our shader.
    GLuint shaderprogram = pShader->GetProgram();
    glUseProgram( shaderprogram );

    // VBO and Attributes.
    {
        glBindBuffer( GL_ARRAY_BUFFER, m_VBO );

        GLint aPosition = glGetAttribLocation( shaderprogram, "a_Position" );
        if( aPosition != -1 )
        {
            glVertexAttribPointer( aPosition, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)offsetof(VertexFormat, m_Position) );
            glEnableVertexAttribArray( aPosition );
        }

        GLint aUV = glGetAttribLocation( shaderprogram, "a_UV" );
        if( aUV != -1 )
        {
            glVertexAttribPointer( aUV, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)offsetof(VertexFormat, m_UV) );
            glEnableVertexAttribArray( aUV );
        }
    }

    // Uniforms for transforms.
    {
        GLint uPosition = glGetUniformLocation( shaderprogram, "u_Position" );
        glUniform2f( uPosition, pos.x, pos.y );

        // Don't leave camera position and projection scale hard-coded, create a camera object and pass them in as arguments.

		GLint uCameraOffset = glGetUniformLocation(shaderprogram, "u_CameraOffset");
		glUniform2f(uCameraOffset, cam->GetCameraTranslation().x * -1, cam->GetCameraTranslation().y * -1);

		GLint uProjectionScale = glGetUniformLocation(shaderprogram, "u_ProjectionScale");
		glUniform2f(uProjectionScale, cam->GetCameraScale().x, cam->GetCameraScale().y);


		GLint auvscale = glGetUniformLocation(shaderprogram, "u_UVScale");

		if (auvscale != -1)
		{

			glUniform2f(auvscale, UVscale.x, UVscale.y);
		}

		GLint a_uvOffset = glGetUniformLocation(shaderprogram, "u_UVOffset");

		if (a_uvOffset != -1)
		{

			glUniform2f(a_uvOffset, UVoffset.x, UVoffset.y);
		}
    }

    // Uniforms for textures.
    {
        glActiveTexture( GL_TEXTURE0 + 0 );
        glBindTexture( GL_TEXTURE_2D, texture );
        GLint uTexture = glGetUniformLocation( shaderprogram, "u_Texture" );
        glUniform1i( uTexture, 0 );
    }

    CheckForGLErrors();

    glDrawArrays( m_PrimitiveType, 0, m_NumVerts );

    CheckForGLErrors();
}

void Mesh::DrawHealthbar(ShaderProgram* pShader, GLuint texture, vec2 pos, vec2 UVoffset, vec2 UVscale, float Health, float maxHealth, Camera * cam, float Red, float Green, float Blue, float Alpha)
{
	CheckForGLErrors();

	// Use our shader.
	GLuint shaderprogram = pShader->GetProgram();
	glUseProgram(shaderprogram);

	// VBO and Attributes.
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

		GLint aPosition = glGetAttribLocation(shaderprogram, "a_Position");
		if (aPosition != -1)
		{
			glVertexAttribPointer(aPosition, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)offsetof(VertexFormat, m_Position));
			glEnableVertexAttribArray(aPosition);
		}

		GLint aUV = glGetAttribLocation(shaderprogram, "a_UV");
		if (aUV != -1)
		{
			glVertexAttribPointer(aUV, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)offsetof(VertexFormat, m_UV));
			glEnableVertexAttribArray(aUV);
		}
	}

	// Uniforms for transforms.
	{
		GLint uPosition = glGetUniformLocation(shaderprogram, "u_Position");
		glUniform2f(uPosition, pos.x, pos.y);

		// Don't leave camera position and projection scale hard-coded, create a camera object and pass them in as arguments.

		GLint uCameraOffset = glGetUniformLocation(shaderprogram, "u_CameraOffset");
		glUniform2f(uCameraOffset, cam->GetCameraTranslation().x * -1, cam->GetCameraTranslation().y * -1);

		GLint uProjectionScale = glGetUniformLocation(shaderprogram, "u_ProjectionScale");
		glUniform2f(uProjectionScale, cam->GetCameraScale().x, cam->GetCameraScale().y);


		GLint auvscale = glGetUniformLocation(shaderprogram, "u_UVScale");

		if (auvscale != -1)
		{

			glUniform2f(auvscale, UVscale.x, UVscale.y);
		}

		GLint health = glGetUniformLocation(shaderprogram, "u_HealthPercent");

		if (health != -1)
		{

			glUniform1f(health, Health / maxHealth);
		}

		GLint HealthBarColor = glGetUniformLocation(shaderprogram, "u_HealthBarColor");

		if (HealthBarColor != -1)
		{
			glUniform4f(HealthBarColor, Red, Blue, Green, Alpha);
		}


		GLint a_uvOffset = glGetUniformLocation(shaderprogram, "u_UVOffset");

		if (a_uvOffset != -1)
		{

			glUniform2f(a_uvOffset, UVoffset.x, UVoffset.y);
		}
	}

	// Uniforms for textures.
	{
		glActiveTexture(GL_TEXTURE0 + 0);
		glBindTexture(GL_TEXTURE_2D, texture);
		GLint uTexture = glGetUniformLocation(shaderprogram, "u_Texture");
		glUniform1i(uTexture, 0);
	}

	CheckForGLErrors();

	glDrawArrays(m_PrimitiveType, 0, m_NumVerts);

	CheckForGLErrors();
}

void Mesh::DrawScore(ShaderProgram* pShader, GLuint texture, vec2 pos, int Texturewidth, int TextureHeight, int FPerRow, int FPerCol, int currentnumber, Camera * cam)
{

	// Set our shader as the currently active one.
	GLuint shaderprogram = pShader->GetProgram();
	glUseProgram(shaderprogram);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	// Get the index of the position attribute in our shader and setup the attribute details.
	GLint aPosition = glGetAttribLocation(shaderprogram, "a_Position");
	if (aPosition != -1)
	{
		glVertexAttribPointer(aPosition, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)offsetof(VertexFormat, m_Position));
		glEnableVertexAttribArray(aPosition);
	}


	GLint uPosition = glGetUniformLocation(shaderprogram, "u_Position");

		glUniform2f(uPosition, pos.x, pos.y);




	GLint uCameraOffset = glGetUniformLocation(shaderprogram, "u_CameraOffset");
	if (uCameraOffset != -1)
	{
		glUniform2f(uCameraOffset, cam->GetCameraTranslation().x * -1, cam->GetCameraTranslation().y * -1);
	}

	GLint uProjectionScale = glGetUniformLocation(shaderprogram, "u_ProjectionScale");
	if (uProjectionScale != -1)
	{
		glUniform2f(uProjectionScale, cam->GetCameraScale().x, cam->GetCameraScale().y);
	}


	GLint uv = glGetAttribLocation(shaderprogram, "a_Uv");
	if (uv != -1)
	{
		glVertexAttribPointer(uv, 2, GL_FLOAT, GL_FALSE, 16, (void*)8);
		glEnableVertexAttribArray(uv);
	}

	GLint uTexture = glGetUniformLocation(shaderprogram, "u_MainTex");

	if (uTexture != -1)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glUniform1i(uTexture, 0);
	}

	GLint uTextWidth = glGetUniformLocation(shaderprogram, "uTextWidth");
	if (uTextWidth != -1)

	{
		glUniform1i(uTextWidth, Texturewidth);
	}


	GLint uTextHeight = glGetUniformLocation(shaderprogram, "uTextHeight");
	if (uTextHeight != -1)

	{
		glUniform1i(uTextHeight, TextureHeight);
	}

	GLint ufPerRow = glGetUniformLocation(shaderprogram, "u_fPerRow");
	if (ufPerRow != -1)

	{
		glUniform1i(ufPerRow, FPerRow);
	}


	GLint ufPerCol = glGetUniformLocation(shaderprogram, "u_fPerCol");
	if (ufPerCol != -1)

	{
		glUniform1i(ufPerCol, FPerCol);
	}


	int number = currentnumber;

	int div = 10;
	int remainder = 0;
	float numOffset = 0;

	while (number)
	{
		remainder = number / div;
		number = number % div;

		int xindex = number % FPerRow;
		int yindex = number / FPerRow;

		number = remainder;

		GLint uxindex = glGetUniformLocation(shaderprogram, "u_Xindex");
		if (uxindex != -1)

		{
			glUniform1i(uxindex, xindex);
		}

		GLint uyindex = glGetUniformLocation(shaderprogram, "u_Yindex");
		if (uyindex != -1)

		{
			glUniform1i(uyindex, yindex);
		}

		GLint uNumOffSet = glGetUniformLocation(shaderprogram, "u_NumOfset");
		if (uNumOffSet != -1)

		{
			glUniform1f(uNumOffSet, numOffset);
		}


		numOffset -= 0.10f;

		// Draw the triangle.
		glDrawArrays(m_PrimitiveType, 0, m_NumVerts);
	}
	// Check for errors.
	CheckForGLErrors();
}


