


template <typename T>
class TSingleton {
public:
    static T* Instance() {
        if(!pInstance)
            pInstance = new T;
        //assert(m_pInstance != nullptr);
        return pInstance;
    }
protected:
    TSingleton();
    ~TSingleton();
private:
    TSingleton(TSingleton const&);
    TSingleton& operator=(TSingleton const&);
    static T* pInstance;
};

template <class T> T* TSingleton<T>::pInstance = nullptr;
