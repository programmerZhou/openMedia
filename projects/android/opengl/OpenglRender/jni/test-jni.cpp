/*!
 *	\file jni adapter source
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <jni.h> 
#include <fcntl.h>
#include <errno.h>
#include <dlfcn.h>

#include "test.h"
#include "um.h"

/*!
 * 	\brief	The Java layout invoke class
 */
static const char *classPathName = "com/example/openglrender/TestCaseActivity";
static jmethodID notifyOpenglDrawCB;
static jclass m_upjni_class;


/*!
*	\brief	pass the surface to native from java, and relationship with windows
*/
struct fields_t {
    jfieldID    surface;	//!< actually in android.view.Surface XXX 
    jfieldID    surface_native;	//!< the surface passed from java
}; 
static fields_t fields;

UM_VideoDecoderCtx* ctx;
UM_VideoRenderCtx* prender;
#define LIB_LENGTH 256

static void* libubmedia = UM_NULL;

//UMSint  (*upeg_adec_init)(const UMSint8* packetName);
//
//UMSint  (*upeg_adec_fini)();
//
//UM_AudioDecoderCtx*     (*upeg_adec_create)(UM_CodecParams* params);
//
//UMSint  (*upeg_adec_destroy)(UM_AudioDecoderCtx* thiz);
//
//UMSint  (*upeg_adec_decode)(UM_AudioDecoderCtx* thiz, UMSint8* buf, UMSint bufLen);
//
//UMSint  (*upeg_adec_reset)(UM_AudioDecoderCtx* thiz);
//
//
//UMSint (*upeg_audio_init)(UM_AudioRenderCtx* thiz, void* callback);
//
//UMSint (*upeg_audio_fini)(UM_AudioRenderCtx* thiz);
//
//UMSint (*upeg_audio_render)(UM_AudioRenderCtx* thiz, UMSint* buf, UMSint bufLen);



/***************************************************************
*    ubmedia video decoder & render interface define
****************************************************************/

UMSint  (*upeg_vdec_init)(UMSint8* packetName);

UMSint  (*upeg_vdec_fini)();

UM_VideoDecoderCtx*     (*upeg_vdec_create)(UM_CodecParams* params);

UMSint  (*upeg_vdec_destroy)(UM_VideoDecoderCtx* thiz);

UMSint  (*upeg_vdec_setSurface)(UM_VideoDecoderCtx* thiz, void* surface);

UMSint  (*upeg_vdec_decode)(UM_VideoDecoderCtx* thiz, UMSint8* buf, UMSint bufLen);

UMSint  (*upeg_vdec_reset)(UM_VideoDecoderCtx* thiz);

void (*upeg_vdec_setCompensateNum)(UM_VideoDecoderCtx* thiz, UMSint frameNumbers);

UMSint (*upeg_vdec_getSurfaceSize)(UM_VideoDecoderCtx* thiz, void* surface, int* width, int* hight);

UMSint (*upeg_glrender_init)();

UMSint (*upeg_glrender_fini)();

UM_VideoRenderCtx* (*upeg_glrender_create)(UM_CodecParams* params);

UMSint (*upeg_glrender_destory)(UM_VideoRenderCtx* thiz);

UMSint (*upeg_glrender_draw)(UM_VideoRenderCtx* thiz, UMSint8* buf, UMSint width, UMSint height);

UMSint (*upeg_glrender_render)(UM_VideoRenderCtx* thiz);

UMSint (*upeg_glrender_resize)(UM_VideoRenderCtx* thiz, UMSint width, UMSint height);

UMSint (*upeg_glrender_pause)(UM_VideoRenderCtx* thiz);

UMSint (*upeg_glrender_resume)(UM_VideoRenderCtx* thiz);


static int s_upeg_get_versionStr(int version, char* verStr, char* ver)
{
    if( (8 < version) &&( version < 11))
    {

        LOGE("it is gingerbread");
        strcpy(verStr, "gingerbread");
        strcpy(ver, "gb");
    }
    else if((10 < version) && (version < 14))
    {
        LOGE("it is honeycomb");
        strcpy(verStr, "honeycomb");
        strcpy(ver, "hc");
    }
    else if((13 < version) && (version < 16))
    {

        LOGE("it is ics");
        strcpy(verStr, "ics");
        strcpy(ver, "ics");
    }
    else if( 15 < version )
    {

        LOGE("it is jellybean");
        strcpy(verStr, "jellybean");
        strcpy(ver, "jb");
    }
    else
        return -1;

    return 0;
}


int upeg_load_media(int codec, const char* pkgname, UMBool isHardWare, const char* model, int version, char* load)
{
    int  len;
    char ver[8];
    char verStr[16];
    char ubmedia[64];
    char paths[LIB_LENGTH] = {0};
    UMBool isHardwareDec = UM_TRUE;

    if(!pkgname) return -1;

    sprintf(paths, "/data/data/%s/lib/", pkgname);
    len = strlen(paths);

    // mapping the version number to verdion code.
    // if the verion code is less than 9(gingerbread), it should choose software decoder.
    memset(verStr, 0 , 16);
    memset(ubmedia, 0 , 64);
    if(version > 8)
        s_upeg_get_versionStr(version, verStr, ver);

    //according to the model information choose ubmedia library.
    memset(ubmedia, 0 , 64);
    if(isHardWare == UM_TRUE)
    {
        // generally,it will use the mobile phone decoder.
        sprintf(ubmedia, "libubmedia_hw_stagefright_%s.so",verStr);

        strcpy(paths + len, ubmedia);
        strcpy(load, ubmedia);

    }

    if(isHardWare == UM_FALSE)
    {

    	if(codec == 0){
			strcpy(paths + len, "/libubmedia_sw_ffmpeg.so");
			strcpy(load, "libubmedia_sw_ffmpeg.so");
    	}else if(codec == 1){
			strcpy(paths + len, "/libubmedia_sw_opencore.so");
			strcpy(load, "libubmedia_sw_opencore.so");
    	}else if(codec == 2){
    		strcpy(paths + len, "/libubmedia_hw_stagefright.so");
    					strcpy(load, "libubmedia_hw_stagefright.so");
    	}else if(codec == 3){
    		strcpy(paths + len, "/libubmedia_hw_stagefright_jellybean43.so");
    					strcpy(load, "libubmedia_hw_stagefright_jellybean43.so");
    	}else{
    		LOGE("libubmedia decode have not define");
    	}
    }

    LOGE("libubmedia have been loaded %s\n", paths);
    libubmedia = dlopen(paths, RTLD_NOW);

    if(!libubmedia)
    {
        LOGE("Unable to load %s: %s\n", paths, dlerror());
        return -1;
    }

//    upeg_adec_init = dlsym(libubmedia, "um_adec_init");
//    upeg_adec_fini = dlsym(libubmedia, "um_adec_fini");
//    upeg_adec_create = dlsym(libubmedia, "um_adec_create");
//    upeg_adec_destroy = dlsym(libubmedia, "um_adec_destroy");
//    upeg_adec_decode = dlsym(libubmedia, "um_adec_decode");
//    upeg_adec_reset = dlsym(libubmedia, "um_adec_reset");

    upeg_vdec_init = (UMSint (*)(UMSint8* ) )dlsym(libubmedia, "um_vdec_init");
    LOGE("libubmedia have been loaded upeg_vdec_init %p\n", upeg_vdec_init);
    upeg_vdec_fini = (UMSint (*)())dlsym(libubmedia, "um_vdec_fini");
    upeg_vdec_create = ( UM_VideoDecoderCtx* (*)(UM_CodecParams*) )dlsym(libubmedia, "um_vdec_create");
    upeg_vdec_destroy = (UMSint (*)(UM_VideoDecoderCtx* ) )dlsym(libubmedia, "um_vdec_destroy");
    upeg_vdec_setSurface = (UMSint  (*)(UM_VideoDecoderCtx* , void* ))dlsym(libubmedia, "um_vdec_setSurface");
    upeg_vdec_decode = (UMSint  (*)(UM_VideoDecoderCtx* , UMSint8* , UMSint ))dlsym(libubmedia, "um_vdec_decode");

    upeg_glrender_init = ( UMSint (*)() )dlsym(libubmedia, "um_glrender_init");
    upeg_glrender_fini = ( UMSint (*)() )dlsym(libubmedia, "um_glrender_fini");
    upeg_glrender_create = ( UM_VideoRenderCtx* (*)(UM_CodecParams* ) )dlsym(libubmedia, "um_glrender_create");
    upeg_glrender_destory = ( UMSint (*)(UM_VideoRenderCtx* ) )dlsym(libubmedia, "um_glrender_destory");
    upeg_glrender_draw = ( UMSint (*)(UM_VideoRenderCtx* , UMSint8* , UMSint , UMSint ) )dlsym(libubmedia, "um_glrender_draw");
    upeg_glrender_render = ( UMSint (*)(UM_VideoRenderCtx* ) )dlsym(libubmedia, "um_glrender_render");
    upeg_glrender_resize = ( UMSint (*)(UM_VideoRenderCtx* , UMSint , UMSint ) )dlsym(libubmedia, "um_glrender_resize");
    upeg_glrender_pause = ( UMSint (*)(UM_VideoRenderCtx* ))dlsym(libubmedia, "um_glrender_pause");
    upeg_glrender_resume = ( UMSint (*)(UM_VideoRenderCtx* ) )dlsym(libubmedia, "um_glrender_resume");

    return 0;
}

void upeg_unload_media(){

    if(libubmedia != UM_NULL)
    {
        dlclose(libubmedia);
        libubmedia = UM_NULL;
    }
}


/*!
*	\brief	simple interface invoke without implement
*/
JNIEXPORT jint JNICALL jni_um_vdec_init(JNIEnv * env, jobject obj, jint codec, jstring pkg, jint version)
{
	const char* pkgnameStr = env->GetStringUTFChars(pkg, NULL);
	char load[64];
	upeg_load_media(codec, pkgnameStr, false, NULL, version, load);

	upeg_vdec_init((UMSint8* )pkgnameStr);

	env->ReleaseStringUTFChars(pkg, pkgnameStr);
	return  0;

}


JNIEXPORT void jni_um_vrend_renderFrame(JNIEnv *env, jobject thiz)
{
	LOGE("Hello jni_um_vrend_renderFrame! ");
	upeg_glrender_render((UM_VideoRenderCtx*)prender);
    return;
}

JNIEXPORT jint JNICALL jni_um_vrend_resize(JNIEnv * env, jobject obj, jint width, jint height)
{
	LOGE("Hello jni_um_vrend_resize! ");
	upeg_glrender_resize(prender, width, height);
	return 0;
}

JNIEXPORT jint JNICALL jni_um_vrend_create(JNIEnv * env, jobject obj)
{
	LOGE("Hello jni_um_vrend_create! ");
	prender = upeg_glrender_create(NULL);
	return 0;
}

JNIEXPORT jint JNICALL jni_um_vrend_destory(JNIEnv* env, jobject clazz)
{
	LOGE("Hello jni_um_vrend_destory! ");
	upeg_glrender_destory((UM_VideoRenderCtx*)prender);
	LOGE("Hello jni_um_vrend_destory! exit");
}

/*!
*	\brief	set the params for video player 
*/
JNIEXPORT jint JNICALL jni_um_vdec_create(JNIEnv * env, jobject obj, jint type, jint width, jint height)
{
	LOGE("Hello jni_um_vdec_create! type is ");
	UMCodecParams params;

	if(type == 1)
		params.codec= CODEC_UID_H264;//h264
	else if(type == 2)
		params.codec= CODEC_UID_MPEG4;//mpeg-4
	else
		LOGE("Hello jni_um_vdec_create! type error");
	
	params.width = width;
	params.height = height;

	LOGE("Hello jni_um_vdec_create! type is ");
	ctx = upeg_vdec_create(&params);

}

/*!
*	\brief	static function, it will be invoke by jni_um_video_surface_set
*/
static void* get_surface(JNIEnv* env, jobject clazz)
{

    void* p = (void* )env->GetIntField(clazz, fields.surface_native);
    return (p);

}

/*!
*	\brief	relationship surface with native window,the surface is passed from java
*/
JNIEXPORT jint JNICALL jni_um_vdec_setSurface(JNIEnv * env, jobject obj)
{
	LOGE("Hello jni_um_video_surface_set!");
	
	jobject surface = env->GetObjectField(obj, fields.surface);
	LOGE("jni_um_vdec_setSurface(), surface=%p", surface);
	if (surface != NULL) 
	{
		void* native_surface = get_surface(env, surface);
				return upeg_vdec_setSurface(ctx, native_surface);
    }
	
	return -1;
}


static void NV12toI420(UMSint8* yuv420sp, UMSint8* yuv420, int width, int height)
{
	//LOGD("Hello NV21toI420!");

    if (yuv420sp == NULL ||yuv420 == NULL) return;

    int framesize = width*height;
    int i = 0, j = 0;

    //copy y
    for (i = 0; i < framesize; i++)
    {
        yuv420[i] = yuv420sp[i];
    }

    //LOGD("NV21toI420! copy y done");
    i = 0;
    for (j = 0; j < framesize/2; j+=2)
    {
        yuv420[i + framesize*5/4] = yuv420sp[j+framesize];
        i++;
    }

    //LOGD("NV21toI420! copy v done");
    i = 0;
    for(j = 1; j < framesize/2;j+=2)
    {
        yuv420[i+framesize] = yuv420sp[j+framesize];
        i++;
    }

    //LOGD("NV21toI420! copy u done");
}

static void NV21toI420(UMSint8* yuv420sp, UMSint8* yuv420, int width, int height)
{
	//LOGD("Hello NV21toI420!");

    if (yuv420sp == NULL ||yuv420 == NULL) return;

    int framesize = width*height;
    int i = 0, j = 0;

    //copy y
    for (i = 0; i < framesize; i++)
    {
        yuv420[i] = yuv420sp[i];
    }

    //LOGD("NV21toI420! copy y done");
    i = 0;
    for (j = 0; j < framesize/2; j+=2)
    {
        yuv420[i + framesize] = yuv420sp[j+framesize];
        i++;
    }

    //LOGD("NV21toI420! copy v done");
    i = 0;
    for(j = 1; j < framesize/2;j+=2)
    {
        yuv420[i+framesize*5/4] = yuv420sp[j+framesize];
        i++;
    }

    //LOGD("NV21toI420! copy u done");
}

/*!
*	\brief	main decode function,
*	\param	in	it's nal buf
*	\param	len	it's the length of nal
*	\return	if success, it will return UM_SUCCESS
*/
JNIEXPORT jint JNICALL jni_um_vdec_decode(JNIEnv * env, jobject obj, jbyteArray in, jint len)
{

	UMSint ret;
	UMSint offset = 0;
	int totalSize = len;
	UMSint8 i420[576000];

	jbyte * buf = (jbyte*)env->GetByteArrayElements(in, 0);

	LOGE("buf is 0x%x  0x%x 0x%x 0x%x and length  is %d",buf[0],buf[1],buf[2],buf[4],len);

//	do{

//		if(offset >= totalSize) break;

		ret = upeg_vdec_decode(ctx, (UMSint8* )(buf + offset), totalSize - offset);
		LOGE("outWidth is %d and outHeight is %d ", ctx->outWidth, ctx->outHeight);

//		if(ret < 0){
//			LOGE("decode fail..... ret is %d", ret);
//			return -1;
//		}else{
//			offset += ret;
//
//		}

		LOGE("decode done------- ctx->outDataLen is %d", ctx->outDataLen);
//		LOGD("+++++++NV21toI420 entry");
//		NV21toI420(ctx->outData, i420, 800, 480);
//		LOGD("+++++++NV21toI420 entry");

		//LOGD("+++++++NV21toI420 entry");
		//NV12toI420(ctx->outData, i420, 800, 480);
		//LOGD("+++++++NV21toI420 entry");

		//LOGD("+++++++upeg_glrender_draw entry");
		if(ctx->outDataLen == 0) return 0;

		upeg_glrender_draw(prender, ctx->outData, ctx->outWidth, ctx->outHeight);
		//upeg_glrender_draw(prender, i420, ctx->outWidth, ctx->outHeight);
		//LOGD("+++++++upeg_glrender_draw exit");

		 env->CallStaticVoidMethod(m_upjni_class, notifyOpenglDrawCB);

//	}while(ret > 0 && totalSize > 0);


	env->ReleaseByteArrayElements(in, buf, 0); 
	
	return 0;
	
}



/*!
*	\brief	simple interface invoke without implement
*/
JNIEXPORT jint JNICALL jni_um_vdec_destroy(JNIEnv * env, jobject obj)
{

	upeg_vdec_destroy(ctx);
	ctx = NULL;

	upeg_unload_media();
	return 1;
}

/*!
*	\brief	simple interface invoke without implement
*/
JNIEXPORT jint JNICALL jni_um_vdec_fini(JNIEnv * env, jobject obj)
{

	return upeg_vdec_fini();
}

/*!
*	\brief	funciton table from java to native 
*/
static JNINativeMethod methods[] = {
				{"um_vdec_init",		"(ILjava/lang/String;I)I",	(void*)jni_um_vdec_init},
				{"um_vdec_create",		"(III)I",					(void*)jni_um_vdec_create},
				{"um_vdec_decode",		"([BI)I",					(void*)jni_um_vdec_decode},
				{"um_vdec_destroy",		"()I",						(void*)jni_um_vdec_destroy},
				{"um_vdec_fini",		"()I",						(void*)jni_um_vdec_fini},
				{"um_vdec_setSurface",	"()I",						(void*)jni_um_vdec_setSurface},
				{"um_vrend_renderFrame", "()V",						(void*)jni_um_vrend_renderFrame},
				{"um_vrend_create",		"()I",						(void*)jni_um_vrend_create},
				{"um_vrend_destroy",	"()I",						(void*)jni_um_vrend_destory},
				{"um_vrend_resize",		"(II)I",					(void*)jni_um_vrend_resize},
};


/*!
 * 	\brief	Register several native methods for one class.
 */
static int registerNativeMethods(JNIEnv* env, const char* className,
                                JNINativeMethod* gMethods, int numMethods)
{
	jclass clazz;
	fprintf(stderr, "RegisterNatives start for '%s'", className);
	clazz = env->FindClass(className);	//!< get java class instance from my java class
	if (clazz == NULL) {
			fprintf(stderr, "Native registration unable to find class '%s'", className);
			return JNI_FALSE;
	}
	
	m_upjni_class = (jclass) env->NewGlobalRef(clazz);

	notifyOpenglDrawCB  = env->GetStaticMethodID( clazz, "notifyOpenGlDraw", "()V");


	if (env->RegisterNatives(clazz, gMethods, numMethods) < 0) {
			fprintf(stderr, "RegisterNatives failed for '%s'", className);
			return JNI_FALSE;
	}

	return JNI_TRUE;
}

/*!
 * 	\brief Register native methods for all classes we know about.
 * 	\return JNI_TRUE on success.
 */
static int registerNatives(JNIEnv* env)
{
        if (!registerNativeMethods(env, classPathName,
                methods, sizeof(methods) / sizeof(methods[0]))) {
                        return JNI_FALSE;
        }

        return JNI_TRUE;
}


/*!
 * 	\brief switch c and c++
 */
typedef union {
        JNIEnv* env;
        void* venv;
} UnionJNIEnvToVoid;

/*!
 * 	\brief This is called by the VM when the shared library is first loaded.
 */
jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
        UnionJNIEnvToVoid uenv;
        uenv.venv = NULL;
        jint result = -1;
        JNIEnv* env = NULL;

        printf("JNI_OnLoad");

        if (vm->GetEnv(&uenv.venv, JNI_VERSION_1_4) != JNI_OK) {
                fprintf(stderr, "GetEnv failed");
                goto bail;
        }

        env = uenv.env;

        if (registerNatives(env) != JNI_TRUE) {
                fprintf(stderr, "GetEnv failed");
                goto bail;
}

                result = JNI_VERSION_1_4;
bail:
    return result;
}

