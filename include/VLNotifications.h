#pragma once
// Notify all subscriptors
// Send "before" notification
#define INIT_NOTIFY_BEFORE \
	vl::Object nfInfObj; \
	auto nInfPtr = vl::MakePtr(nfInfObj); \
	nfInfObj.Set("before", true);

#define INIT_NOTIFY \
	vl::Object nfInfObj; \
	auto nInfPtr = vl::MakePtr(nfInfObj); \

#define SEND_NOTIFY(observable) \
	observable->Notify(nInfPtr);

#define SEND_NOTIFY_BEFORE(cmd, arg, observable) \
	nfInfObj.Set(cmd, arg); \
	observable->Notify(nInfPtr);

#define NOTIFY_BEFORE(cmd, arg, observable) \
	vl::Object nfInfObj; \
	auto nInfPtr = vl::MakePtr(nfInfObj); \
	nfInfObj.Set(cmd, arg); \
	nfInfObj.Set("before", true); \
	observable->Notify(nInfPtr);

#define NOTIFY_ADD(id, val) \
	nfInfObj.Set(id, val);

// Send "after" notification
#define NOTIFY_AFTER(observable) \
	nfInfObj.RemoveProperty("before"); \
	nfInfObj.Set("after", true); \
	observable->Notify(nInfPtr);