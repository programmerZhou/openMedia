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
static const char *classPathName = "com/example/rgb2yuv/MainActivity";
static jmethodID notifyOpenglDrawCB;
static jclass m_upjni_class;


static fields_t fields;

UM_VideoDecoderCtx* ctx;
UM_VideoRenderCtx* prender;
#define LIB_LENGTH 256


/*!
*	\brief	simple interface invoke without implement
*/
JNIEXPORT jint JNICALL jni_um_vdec_init(JNIEnv * env, jobject obj, jint codec, jstring pkg, jint version)
{
	const char* pkgnameStr = env->GetStringUTFChars(pkg, NULL);

	env->ReleaseStringUTFChars(pkg, pkgnameStr);
	return  0;

}




/*!
*	\brief	funciton table from java to native 
*/
static JNINativeMethod methods[] = {
				{"um_vdec_init",		"(ILjava/lang/String;I)I",	(void*)jni_um_vdec_init},
				{"um_vdec_init",		"(ILjava/lang/String;I)I",	(void*)jni_um_vdec_init},

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
	
	//m_upjni_class = (jclass) env->NewGlobalRef(clazz);
	//notifyOpenglDrawCB  = env->GetStaticMethodID( clazz, "notifyOpenGlDraw", "()V");


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

