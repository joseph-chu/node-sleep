
#include <node.h>

using v8::Exception;
using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Handle;
using v8::Object;
using v8::String;
using v8::Local;
using v8::Value;

#if defined _WIN32 || defined _WIN64
unsigned int sleep(unsigned int seconds)
{
    Sleep(seconds * 1000);
    return 0;
}
int usleep(unsigned __int64 usec)
{
    LARGE_INTEGER li;
    li.QuadPart = -10 * usec;     // negative values for relative time

    HANDLE timer = CreateWaitableTimer(NULL, TRUE, NULL);
    if(!timer) return -1;

    SetWaitableTimer(timer, &li, 0, NULL, NULL, 0);
    WaitForSingleObject(timer, INFINITE);
    CloseHandle(timer);
    return 0;
}
#else
#   include <unistd.h>
#endif


void Sleep (const FunctionCallbackInfo<Value>& info) {
  Isolate* isolate = info.GetIsolate();

  if (info.Length() < 1 || !info[0]->IsUint32()) {
    isolate->ThrowException(Exception::TypeError(
      String::NewFromUtf8(isolate, "Expected number of seconds")));
    return;
  }

  sleep(info[0]->Uint32Value());

  info.GetReturnValue().SetUndefined();
}

void USleep (const FunctionCallbackInfo<Value>& info) {
  Isolate* isolate = info.GetIsolate();

  if (info.Length() < 1 || !info[0]->IsUint32()) {
    isolate->ThrowException(Exception::TypeError(
      String::NewFromUtf8(isolate, "Expected number of microseconds")));
    return;
  }

  usleep(info[0]->Uint32Value());

  info.GetReturnValue().SetUndefined();
}

void init(Local<Object> exports) {
  NODE_SET_METHOD(exports, "sleep", Sleep);
  NODE_SET_METHOD(exports, "usleep", USleep);
}

NODE_MODULE(node_sleep, init)

