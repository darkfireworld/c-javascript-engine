#include"JsObject.h"
#include"JsVm.h"
#include"JsECMAScript.h"


void JsECMAScriptObjectInit(struct JsVm* vm){
	
	
	//初始化vm->Global对象
	struct JsObject* Global =  JsCreateStandardObject(NULL);
	vm->Global = Global;	
	
	JsOFInit(vm);	
	JsArrayInit(vm);
	JsStringInit(vm);
	JsBooleanInit(vm);
	JsNumberInit(vm);
	JsThreadInit(vm);
	JsGlobalInit(vm);
}
