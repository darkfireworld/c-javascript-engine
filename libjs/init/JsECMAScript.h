#ifndef JsECMAScriptH
#define JsECMAScriptH

//Object And Function
void JsOFInit(struct JsVm* vm);
void JsArrayInit(struct JsVm* vm);
void JsStringInit(struct JsVm* vm);
void JsBooleanInit(struct JsVm* vm);
void JsNumberInit(struct JsVm* vm);
void JsThreadInit(struct JsVm* vm);
void JsGlobalInit(struct JsVm* vm);
#endif