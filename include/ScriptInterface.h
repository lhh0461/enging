struct TScriptObj
{
    void *ptr;
}

class CScriptInterface
{
public:
    static int InitEnv();
    //
    static int CallScriptFunc(const char *pModuleName, const char *pFuncName);
public: //类型转换 engine->scrpit
    static TScriptObj *ConvertInt(int64_t val);
    static TScriptObj *NewTuple();
    static int SerTupleItem();

};
