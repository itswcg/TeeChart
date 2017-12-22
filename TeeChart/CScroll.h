// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装器类

#import "C:\\Program Files (x86)\\TeeChart Pro v5 ActiveX Control\\TeeChart5.ocx" no_namespace
// CScroll 包装器类

class CScroll : public COleDispatchDriver
{
public:
	CScroll() {} // 调用 COleDispatchDriver 默认构造函数
	CScroll(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CScroll(const CScroll& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 特性
public:

	// 操作
public:


	// IScroll 方法
public:
	long get_Enable()
	{
		long result;
		InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_Enable(long newValue)
	{
		static BYTE parms[] = VTS_I4;
		InvokeHelper(0x1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_MouseButton()
	{
		long result;
		InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_MouseButton(long newValue)
	{
		static BYTE parms[] = VTS_I4;
		InvokeHelper(0x2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}

	// IScroll 属性
public:

};
