/*
 * Copyright (c) 2007-2012 ubitus Inc. All Rights Reserved.
 *
 * This software is the confidential and proprietary information of
 * ubitus Inc. ("Confidential Information"). You shall not disclose
 * such Confidential Information and shall use it only in accordance
 * with the terms of the license agreement you entered into with ubitus
 * Inc. All unauthorized usages in any manner are expressly prohibited.
 *
 * UBITUS MAKES NO REPRESENTATIONS OR WARRANTIES ABOUT THE SUITABILITY OF
 * THE SOFTWARE, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
 * TO THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE, OR NON-INFRINGEMENT. UBITUS SHALL NOT BE LIABLE FOR
 * ANY DAMAGES SUFFERED BY LICENSEE AS A RESULT OF USING, MODIFYING OR
 * DISTRIBUTING THIS SOFTWARE OR ITS DERIVATIVES.
*/

#include "glrender_impl.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <time.h>

//#include "um_circle_queue.h"
//#include "um_sys_mutex.h"

#define LOG_ERR(...) ((void)0)


#define PIX_FORMAT_RATE 1.5
#define PIX_FORMAT GL_LUMINANCE
#define PIX_TYPE GL_UNSIGNED_BYTE


static void GLESCreateFramebuffer(int width, int height);
static void GLESDestroyFramebuffer();
static void GLESSetup();

static ESRendererInfo* m_glview_pRenderInfo;
static ESRendererData* m_glview_pRenderData;

static GLuint g_renderbuffer = 0;
static GLuint g_framebuffer = 0;

static GLuint g_textureAtlas = 0;
static GLfloat g_texturePoints[8];
static GLfloat g_points[8];

static unsigned char* m_data=0;
static int m_firstFrameArrived = -1;

static int g_init = 0;

static void* pOpenglRenderBufferCQ = NULL;

static void checkGlError(const char* op)
{
	GLint error=0;
	error = glGetError();

	if(error!=0)
		LOG_ERR("after %s() glError (0x%x)\n", op, error);

}
 
static bool linkProgram(int prog)
{
    GLint status;

    glLinkProgram(prog);

    glGetProgramiv(prog, GL_LINK_STATUS, &status);
    if (status == 0)
        return false;

    return true;
}

static GLuint loadShader ( GLenum type, const char *shaderSrc )
{

	GLuint shader;
	GLint compiled;

	// Create the shader object
	shader = glCreateShader ( type );

	if ( shader == 0 )
	{
//		LOG_ERR("loadShader(),  shader error");
		return 0;
	}
	// Load the shader source
	glShaderSource(shader, 1, &shaderSrc, NULL);

	// Compile the shader
	glCompileShader(shader);

	// Check the compile status
	glGetShaderiv ( shader, GL_COMPILE_STATUS, &compiled );

	if ( !compiled )
	{
		GLint infoLen = 0;

		glGetShaderiv ( shader, GL_INFO_LOG_LENGTH, &infoLen );

		if ( infoLen > 1 )
		{
			char* infoLog = (char*)malloc (sizeof(char) * infoLen );

			glGetShaderInfoLog ( shader, infoLen, NULL, infoLog );

			free ( infoLog );
		}

		glDeleteShader ( shader );
		return 0;
	}

	return shader;

}

// Create an OpenGL ES 2.0 context
void GLESInit()
{
	if(g_init == 1)
	{
//		LOGE("GLESInit(), not need to init again.");
		return;
	}
	g_init = 1;

	//LOGE("GLESInit() enter width= %d, height=%d", width, height);
	m_firstFrameArrived = -1;
	m_data = 0;
	m_glview_pRenderInfo = (ESRendererInfo *)malloc(sizeof(ESRendererInfo));
	memset(m_glview_pRenderInfo, 0, sizeof(ESRendererInfo));

	m_glview_pRenderData = (ESRendererData *)malloc(sizeof(ESRendererData));
	memset(m_glview_pRenderData, 0, sizeof(ESRendererData));
	

	m_glview_pRenderInfo->program = glCreateProgram();
	checkGlError("glCreateProgram");
	GLbyte vShaderStr[] =
		"precision highp float;\n"
		"precision highp int;\n"
		"attribute vec4 vertexCoords;\n"
		"attribute vec2 textureCoords;\n"
		"varying vec2 f_textureCoords;\n"
		"void main() {\n"
		" f_textureCoords = textureCoords;\n"
		"  gl_Position = vertexCoords;\n"
		"}\n";

	GLbyte fShaderStr[] =

			"precision highp float;\n"
			"precision highp int;\n"
			"varying vec2 f_textureCoords;\n"
			"uniform sampler2D yTexture;\n"
			"uniform sampler2D uTexture;\n"
			"uniform sampler2D vTexture;\n"
			"void main()\n"
			"{\n"
				//"float yuv0;\n"
				//"float yuv1;\n"
				//"float yuv2;\n"
				"mediump vec3 yuv;\n"
				"lowp vec3 rgb;\n"
				"vec4 color;\n"
				"vec4 colorY;\n"
				"vec4 colorU;\n"
				"vec4 colorV;\n"

				//test begins
				"yuv.x = texture2D(yTexture, f_textureCoords).r;\n"
				"yuv.y = texture2D(uTexture, f_textureCoords).r - 0.5;\n"
				"yuv.z = texture2D(vTexture, f_textureCoords).r - 0.5;\n"
				"rgb = mat3(      1,       1,       1,    0, -.21482, 2.12798,  1.28033, -.38059,       0) * yuv;\n"
				"gl_FragColor = vec4(rgb, 1);\n"	               
				//test ends
				
				//"colorY = texture2D(yTexture, f_textureCoords);\n"
				//"colorU = texture2D(uTexture, f_textureCoords);\n"
				//"colorV = texture2D(vTexture, f_textureCoords);\n"

				//"color.r=1.164*(yuv0-16)+2.018*(yuv1-128);\n"
				//"color.g=1.164*(yuv0-16)-0.813*(yuv2-128)-0.391*(yuv1-128);\n"
				//"color.b=1.164*(yuv0-16)+1.596*(yuv2-128);\n"

				//"color = colorY+colorU+colorV;\n"

			     //"gl_FragColor = colorY;\n"
			     //"gl_FragColor = color;\n"
			"}\n";

	GLuint vertexShader;
	GLuint fragmentShader;

	// Load the vertex/fragment shaders
	vertexShader = loadShader(GL_VERTEX_SHADER, (const char *)vShaderStr);
	if(vertexShader == 0)
		LOG_ERR("load vertex shaders error");

	fragmentShader = loadShader(GL_FRAGMENT_SHADER, (const char *)fShaderStr);
	if(fragmentShader == 0)
	{
		glDeleteShader( vertexShader );
		LOG_ERR("load fragment shaders error");
	}

	glCompileShader(fragmentShader);
	checkGlError("glCompileShader");
	glAttachShader ( m_glview_pRenderInfo->program, vertexShader );
	glAttachShader ( m_glview_pRenderInfo->program, fragmentShader );

	if(!linkProgram(m_glview_pRenderInfo->program))
	{
		LOG_ERR("linkProgram error");
	}

	glUseProgram(m_glview_pRenderInfo->program);
	m_glview_pRenderInfo->positionLoc = glGetAttribLocation ( m_glview_pRenderInfo->program, "vertexCoords" );
	m_glview_pRenderInfo->texCoordLoc = glGetAttribLocation ( m_glview_pRenderInfo->program, "textureCoords" );
	m_glview_pRenderInfo->samplerLoc = glGetUniformLocation ( m_glview_pRenderInfo->program, "texture" );
	glUniform1i(m_glview_pRenderInfo->samplerLoc, 0);


	//GLESResize(width, height);
	checkGlError("GLESInit");

	if(NULL == pOpenglRenderBufferCQ)
	{
//		LOGE("GLESInit(), um_cq_create()");
		pOpenglRenderBufferCQ = um_cq_create(6);
	}

//	LOGE("GLESInit() leave");
}

void GLESRelease()
{
	LOG_ERR("GLESRelease() enter");

	if(g_init == 0)
	{
		LOG_ERR("GLESRelease(), not need to release again.");
		return;
	}
	
	if(m_glview_pRenderData)
	{
		free(m_glview_pRenderData);
	}
	for(int i=0;i<3;i++)
	{
		glDeleteTextures(1, &m_glview_pRenderInfo->textureId[i]);
	}

	glDeleteProgram(m_glview_pRenderInfo->program);

	if(m_data)
	{
		free(m_data);
		m_data = 0;
	}
	GLESDestroyFramebuffer();
	m_firstFrameArrived = -1;

	g_init = 0;

	um_cq_stop(pOpenglRenderBufferCQ);
	um_cq_destroy(pOpenglRenderBufferCQ);
	pOpenglRenderBufferCQ = NULL;

//	LOGE("GLESRelease() leave");
}

void GLESResize(int width, int height)
{
	LOG_ERR("GLESResize() width=%d, height=%d", width, height);

	GLESCreateFramebuffer(width, height);

	GLESSetup(); 
}

static void GLESSetup()
{
	LOG_ERR("GLESSetup");  

	for(int i=0;i<3;i++)
	{
		glGenTextures(1, &(m_glview_pRenderInfo->textureId[i]));
		glBindTexture(GL_TEXTURE_2D, m_glview_pRenderInfo->textureId[i]);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);		
		//glGenerateMipmap(GL_TEXTURE_2D);
		
	}
}

static int GetSize(int size)
{
	int temp = 32;
	while(temp<size)
		temp *= 2;

	return temp;
}

void GLESLoadDrawData(char *data, int width, int height)
{
	//LOG_ERR("GLESLoadDrawData()load draw data:%d %d", width, height);
	m_firstFrameArrived = 1;

		//分三路显示：
		//数据:char *pData;
		//y:起点:pData 长度:w*h
		//u:起点:pData+w*h 长度:w*h/4
		//v:起点:pData+w*h+w*h/4 长度:w*h/4
		
		//	FILE *fp = fopen("/sdcard/yuv.dat", "ab+");
		//						fwrite(m_glview_pRenderData->pData[0], 1, width * height, fp);
		//						fclose(fp);
	//	LOGE("m_glview_pRenderData->pData = %d",m_glview_pRenderData->pData);

		int isFull = um_cq_isFull(pOpenglRenderBufferCQ);
		
	//	LOGE("GLESLoadDrawData(), is full=%d", isFull);	
		if(!isFull)
		{
			memcpy(m_glview_pRenderData->y, data, width * height);
			memcpy(m_glview_pRenderData->u, data + width * height, width * height / 4);
			memcpy(m_glview_pRenderData->v, data + width * height + width * height / 4, width * height / 4);
			m_glview_pRenderData->height = height;
			m_glview_pRenderData->width= width;

	//		LOGE("GLESLoadDrawData() um_cq_pushData start");	
			um_cq_pushData(pOpenglRenderBufferCQ, (char*)(m_glview_pRenderData), sizeof(*m_glview_pRenderData), (unsigned int)0);
	//		LOGE("GLESLoadDrawData() um_cq_pushData end");	
		}
		else
		{
			//LOGE("GLESLoadDrawData, isFull=%d", isFull);
			//upui_gc_logger(UPUI_PER_LOG_FLAG_DROP);
		}
}

void GLESRender(void)
{
	pUmCQEntry pEntry;
	int count = um_cq_counter(pOpenglRenderBufferCQ);
	if(0 == count)
	{
		usleep(10*1000);
		return;
	}

	while(count-- > 3)
	{
		if(UM_NULL == (pEntry = um_cq_popFilledEntry(pOpenglRenderBufferCQ))) return;

		/* push back then */
		um_cq_pushEmptyEntry(pOpenglRenderBufferCQ, pEntry);
	}
	
	if(UM_NULL == (pEntry = um_cq_popFilledEntry(pOpenglRenderBufferCQ))) return;

	ESRendererData* data = (ESRendererData*)pEntry->dataHeader;
	int width = data->width;
	int height = data->height;

	if(m_firstFrameArrived < 0)
	{
//		LOGE("GLESRender() line421 leave");
		//upui_gc_logger(UPUI_PER_LOG_FLAG_DROP);
		return;
	}


	static GLfloat vVertices[] =
	{
		-1.f, 1.f, -1.f, -1.f, 1.f, 1.f, 1.f, -1.f
	};
	static GLfloat textureCoords[] = {0, 0, 0, 1.f, 1.f, 0, 1.f, 1.f};
	static GLushort indices[] = {0, 1, 2, 3, 2, 1};

	glClearColor(1.f, 0, 0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glVertexAttribPointer ( m_glview_pRenderInfo->positionLoc, 2, GL_FLOAT, GL_FALSE, 0, vVertices );
	glVertexAttribPointer ( m_glview_pRenderInfo->texCoordLoc, 2, GL_FLOAT, GL_FALSE, 0, textureCoords );
	glEnableVertexAttribArray ( m_glview_pRenderInfo->positionLoc );
	glEnableVertexAttribArray ( m_glview_pRenderInfo->texCoordLoc );

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);


	int m_loc=-1;
	//LOGE("GLESRender() Texture 1 g_iWidth=%d, g_iHeight=%d", g_iWidth, g_iHeight);
	// Bind the texture
	glActiveTexture ( GL_TEXTURE0 );
	glBindTexture(GL_TEXTURE_2D, m_glview_pRenderInfo->textureId[0]);
	m_loc=glGetUniformLocation(m_glview_pRenderInfo->program,"yTexture");
	glUniform1i(m_loc,0);
	//checkGlError("glUniform1i1");
	glTexImage2D(GL_TEXTURE_2D, 0, PIX_FORMAT, width, height, 0, PIX_FORMAT, PIX_TYPE, &data->y);

	//LOGE("Texture 2");
	glActiveTexture ( GL_TEXTURE1 );
	glBindTexture(GL_TEXTURE_2D, m_glview_pRenderInfo->textureId[1]);
    m_loc=glGetUniformLocation(m_glview_pRenderInfo->program,"uTexture");
    glUniform1i(m_loc,1);
    //checkGlError("glUniform1i3");
	glTexImage2D(GL_TEXTURE_2D, 0, PIX_FORMAT, width / 2, height / 2, 0, PIX_FORMAT, PIX_TYPE, &data->u);

	//LOGE("Texture 3");
	glActiveTexture ( GL_TEXTURE2 );
	glBindTexture(GL_TEXTURE_2D, m_glview_pRenderInfo->textureId[2]);
    m_loc=glGetUniformLocation(m_glview_pRenderInfo->program,"vTexture");
    glUniform1i(m_loc,2);
    //checkGlError("glUniform1i2");
	glTexImage2D(GL_TEXTURE_2D, 0, PIX_FORMAT, width / 2, height / 2, 0, PIX_FORMAT, PIX_TYPE, &data->v);

	//glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices );
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	//upui_gc_logger(UPUI_PER_LOG_FLAG_AFTERRESENT);

	um_cq_pushEmptyEntry(pOpenglRenderBufferCQ, pEntry);

	m_firstFrameArrived = 0;
}

void GLESPause()
{
}

void GLESResume()
{
}

static void GLESCreateFramebuffer(int width, int height)
{
//	LOGE("GLESCreateFramebuffer:%d %d", width, height);
	glViewport(0, 0, width, height);
	glScissor(0, 0, width, height);
}

static void GLESDestroyFramebuffer()
{
}


