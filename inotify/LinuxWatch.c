
#define _XOPEN_SOURCE 500
#include "LinuxWatch.h"
#include <sys/inotify.h>
#include <limits.h>
#include <ftw.h>
#include <unistd.h>
#include <string.h>

#define BUF_LEN (10 * (sizeof(struct inotify_event) + NAME_MAX + 1))
JNIEXPORT void JNICALL Java_watch_LinuxWatch_read
(JNIEnv *env, jobject object, jint fileDesc, jobject eventQue) {

  // Prepare to access object of FSEvent.
  jclass cls_fe = (*env)->FindClass(env, "watch/Wevent");
  jmethodID con_fe = (*env)->GetMethodID(env, cls_fe,"<init>","()V");
  jfieldID CREATE_fe = (*env)->GetStaticFieldID(env, cls_fe, "CREATE", "I");
  jint CREATE = (*env)->GetStaticIntField(env, cls_fe, CREATE_fe);
  jfieldID DELETE_fe = (*env)->GetStaticFieldID(env, cls_fe, "DELETE", "I");
  jint DELETE = (*env)->GetStaticIntField(env, cls_fe, DELETE_fe);
  jfieldID DELETE_SELF_fe = (*env)->GetStaticFieldID(env, cls_fe, "DELETE_SELF", "I");
  jint DELETE_SELF = (*env)->GetStaticIntField(env, cls_fe, DELETE_SELF_fe);
  jfieldID MODIFY_fe = (*env)->GetStaticFieldID(env, cls_fe, "MODIFY", "I");
  jint MODIFY = (*env)->GetStaticIntField(env, cls_fe, MODIFY_fe);
  jfieldID MOVED_FROM_fe = (*env)->GetStaticFieldID(env, cls_fe, "MOVED_FROM", "I");
  jint MOVED_FROM = (*env)->GetStaticIntField(env, cls_fe, MOVED_FROM_fe);
  jfieldID MOVED_TO_fe = (*env)->GetStaticFieldID(env, cls_fe, "MOVED_TO", "I");
  jint MOVED_TO = (*env)->GetStaticIntField(env, cls_fe, MOVED_TO_fe);
  jfieldID watch_fe = (*env)->GetFieldID(env, cls_fe, "watch", "I"); 
  jfieldID file_fe = (*env)->GetFieldID(env, cls_fe, "file", "Ljava/lang/String;"); 
  jfieldID type_fe = (*env)->GetFieldID(env, cls_fe, "type", "I"); 
  jfieldID cookie_fe = (*env)->GetFieldID(env, cls_fe, "cookie", "I"); 
  jfieldID isDir_fe = (*env)->GetFieldID(env, cls_fe, "isDir", "Z"); 
  jclass cls_queue = (*env)->FindClass(env, "java/util/LinkedList");
  jmethodID queue_offer = (*env)->GetMethodID(env, cls_queue, "offer", "(Ljava/lang/Object;)Z"); 

  char buf[BUF_LEN];
  ssize_t numRead;
  char *p;
  struct inotify_event *event;
  numRead = read(fileDesc, buf, BUF_LEN);
  // printf("read: %d\n", numRead);
  if (numRead == 0 || numRead == -1)
    return;
  /* Process all of the events in buffer returned by read() */
  for (p = buf; p < buf + numRead; ) {
    event = (struct inotify_event *)p;
    if ((event->mask & IN_CREATE) ||
	(event->mask & IN_DELETE) ||
	// (event->mask & IN_DELETE_SELF) ||
	(event->mask & IN_MODIFY) ||
	(event->mask & IN_MOVED_FROM) ||
	(event->mask & IN_MOVED_TO) ||
	(event->mask & IN_ISDIR)) {
      jobject obj_fe = (*env)->NewObject(env, cls_fe, con_fe);  
      (*env)->SetIntField(env, obj_fe, watch_fe, event->wd);
      jstring file;
      if (event->len == 0)
	file = (*env)->NewStringUTF(env, "");
      else
	file = (*env)->NewStringUTF(env, event->name);
      (*env)->SetObjectField(env, obj_fe, file_fe, file);
      (*env)->SetIntField(env, obj_fe, cookie_fe, event->cookie);
      (*env)->SetIntField(env, obj_fe, type_fe, -1);
      if (event->mask & IN_CREATE) 
	(*env)->SetIntField(env, obj_fe, type_fe, CREATE);
      if (event->mask & IN_DELETE) 
	(*env)->SetIntField(env, obj_fe, type_fe, DELETE);
      //if (event->mask & IN_DELETE_SELF)
      //(*env)->SetIntField(env, obj_fe, type_fe, DELETE_SELF);
      if (event->mask & IN_MODIFY) 
	(*env)->SetIntField(env, obj_fe, type_fe, MODIFY);
      if (event->mask & IN_MOVED_FROM) 
	(*env)->SetIntField(env, obj_fe, type_fe, MOVED_FROM);
      if (event->mask & IN_MOVED_TO) 
	(*env)->SetIntField(env, obj_fe, type_fe, MOVED_TO);
      if (event->mask & IN_ISDIR)
	(*env)->SetBooleanField(env, obj_fe, isDir_fe, 1);
      else
	(*env)->SetBooleanField(env, obj_fe, isDir_fe, 0);
      (*env)->CallBooleanMethod(env, eventQue, queue_offer, obj_fe);
    }
    p += sizeof(struct inotify_event) + event->len;
  }
}

JNIEXPORT jint JNICALL Java_watch_LinuxWatch_init
(JNIEnv *env, jobject object) {
  int fileDesc;
  fileDesc = inotify_init();
  return fileDesc;
}

JNIEXPORT jboolean JNICALL Java_watch_LinuxWatch_destory
(JNIEnv *env, jobject object, jint fileDesc) {
  jboolean ok = JNI_TRUE;
  int ret = close(fileDesc);
  if (ret == -1)
    ok = JNI_FALSE;
  return ok;
}

int fd;
jobject w;
jobject p;
JNIEnv *e;
int travelTree(const char *pathName, const struct stat *sbuf, int type, struct FTW *ftwb) {
  if (type == FTW_D) {
    jclass cls_int = (*e)->FindClass(e, "java/lang/Integer");
    jmethodID con_int = (*e)->GetMethodID(e, cls_int, "<init>", "(I)V");
    jclass cls_map = (*e)->FindClass(e, "java/util/HashMap");
    jmethodID map_put = (*e)->GetMethodID(e, cls_map, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;"); 
    int mask = IN_CREATE | IN_DELETE | IN_MODIFY | IN_MOVED_TO | IN_MOVED_FROM;
    int wd = inotify_add_watch(fd, pathName, mask);
    jobject oi = (*e)->NewObject(e, cls_int, con_int, wd);
    jstring os = (*e)->NewStringUTF(e, pathName);
    (*e)->CallObjectMethod(e, w, map_put, oi, os);
    (*e)->CallObjectMethod(e, p, map_put, os, oi);
    //printf("%s\t*\n", pathName);
  } else {
    //printf("%s\n", pathName);
  }
  return 0;
}

JNIEXPORT void JNICALL Java_watch_LinuxWatch_add
(JNIEnv *env, jobject object, jstring pathName, jint fileDesc, jobject watches, jobject pathes) {
  // Get the path name of a directory
  const char *str = (*env)->GetStringUTFChars(env, pathName, NULL);
  char pn[512];
  strcpy(pn, str);
  (*env)->ReleaseStringUTFChars(env, pathName, str);

  // Travel the subtree an install notifier for each directory
  fd = fileDesc;
  e = env;
  w = watches;
  p = pathes;
  int flags = FTW_MOUNT | FTW_PHYS;
  nftw(pn, travelTree, 10, flags);
}

JNIEXPORT void JNICALL Java_watch_LinuxWatch_delete
(JNIEnv *env, jobject object, jint fileDesc, jint watch) {
  inotify_rm_watch(fileDesc, watch);
}
