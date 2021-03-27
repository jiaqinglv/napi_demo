#include <stdio.h>
#include <assert.h>
#include <node_api.h>
#include <stdlib.h>


typedef struct {
  napi_async_work work;
  napi_deferred deferred;
  char *data;
} AddonData;

// 执行异步函数,这里不是主线程
void OnExecuteWork (napi_env env, void *data) { 
    AddonData *addon_data = (AddonData*)data;

    //所执行的任务，*(addon_data->data)输入的数据
    printf("%s\n",addon_data->data);
}
// 执行异步函数接收，这里同样不是主线程
void OnWorkComplete (napi_env env, napi_status status, void *data) { 
    assert(status == napi_ok);
    AddonData *addon_data = (AddonData*)data;
    status = napi_delete_async_work (env, addon_data->work); // 释放异步工作
    assert(status == napi_ok);
    //内存释放掉
    free(addon_data);
}

napi_value Log (napi_env env, napi_callback_info info) {
    //插件数据
    AddonData *addon_data = (AddonData*)malloc(sizeof(AddonData)); 
    //执行状态
    napi_status status;

    napi_value argv[1];
    size_t argc = 1; 
    char *str = (char*)malloc(1024*sizeof(char)); 
    size_t str_len;
    status = napi_get_cb_info(env, info, &argc, argv, NULL, NULL);
    assert(status == napi_ok);
    status = napi_get_value_string_utf8(env, argv[0], str, 1024*sizeof(char), &str_len);
    assert(status == napi_ok);

    
    napi_value work_name;
    addon_data->data = str;
    

    status = napi_create_string_utf8 (env, "log_work", NAPI_AUTO_LENGTH, &work_name);
    assert(status == napi_ok);
    status = napi_create_async_work (env, NULL, work_name, OnExecuteWork, OnWorkComplete, addon_data, &(addon_data->work)); // 创建异步方法
    assert(status == napi_ok);
    status = napi_queue_async_work (env, addon_data->work); // 将异步方法放入执行队列中
    assert(status == napi_ok);
    return NULL;
}

//模块初始化并注册
NAPI_MODULE_INIT(){
    //日志打印函数
    napi_value log_fn;
    //执行状态
    napi_status status;

    status = napi_create_function(env, NULL, 0, Log, NULL, &log_fn);
    assert(status == napi_ok);
    status = napi_set_named_property(env, exports, "log", log_fn);
    assert(status == napi_ok);
    

    return exports;
}

// 模块注册
// NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)

// 如果模块将在Node.js进程的生存期内多次加载，请使用NAPI_MODULE_INIT宏初始化模块